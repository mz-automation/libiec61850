/*
 *  ethernet_bsd.c
 *
 *  Copyright 2013 Michael Zillgith, contributed to the project by Michael Clausen (School of engineering Valais).
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>
#include <net/bpf.h>
#include <ifaddrs.h>
#include <net/if_dl.h>

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdio.h>

#include "lib_memory.h"
#include "hal_ethernet.h"

struct sEthernetSocket {
    int bpf;                        /* BPF device handle. */
    uint8_t *bpfBuffer;             /* Pointer to the BPF reception buffer. */
    int bpfBufferSize;              /* Actual size of the BPF reception buffer. */
    uint8_t *bpfPositon;            /* Actual read pointer on the BPF reception buffer. */
    uint8_t *bpfEnd;                /* Pointer to the end of the BPF reception buffer. */
    struct bpf_program bpfProgram;  /* BPF filter machine code program. */
};

struct sEthernetHandleSet {
    struct pollfd *handles;
    int nhandles;
};

EthernetHandleSet
EthernetHandleSet_new(void)
{
    EthernetHandleSet result = (EthernetHandleSet) GLOBAL_MALLOC(sizeof(struct sEthernetHandleSet));

    if (result != NULL) {
        result->handles = NULL;
        result->nhandles = 0;
    }

    return result;
}

void
EthernetHandleSet_addSocket(EthernetHandleSet self, const EthernetSocket sock)
{
    if (self != NULL && sock != NULL) {
        int i = self->nhandles++;
        self->handles = realloc(self->handles, self->nhandles * sizeof(struct pollfd));
            
        self->handles[i].fd = sock->bpf;
        self->handles[i].events = POLLIN;
    }
}

void
EthernetHandleSet_removeSocket(EthernetHandleSet self, const EthernetSocket sock)
{
    if ((self != NULL) && (sock != NULL)) {
        unsigned i;
        for (i = 0; i < self->nhandles; i++) {
            if (self->handles[i].fd == sock->bpf) {
                memmove(&self->handles[i], &self->handles[i+1], sizeof(struct pollfd) * (self->nhandles - i - 1));
                self->nhandles--;
                return;
            }
        }
    }
}

int
EthernetHandleSet_waitReady(EthernetHandleSet self, unsigned int timeoutMs)
{
    int result;

    if ((self != NULL) && (self->nhandles >= 0)) {
        result = poll(self->handles, self->nhandles, timeoutMs);
    }
    else {
       result = -1;
    }

    return result;
}

void
EthernetHandleSet_destroy(EthernetHandleSet self)
{
    if (self->nhandles)
        free(self->handles);

    GLOBAL_FREEMEM(self);
}
int
activateBpdFilter(EthernetSocket self)
{
    return ioctl(self->bpf, BIOCSETF, &self->bpfProgram);
}

static int
setBpfEthernetAddressFilter(EthernetSocket self, uint8_t *addr)
{
    if (addr)
    {
        /* Enable Ethernet address filter. */
        self->bpfProgram.bf_insns[0].k = 1;

        /* Copy the address into the filter code. */
        memcpy((void *)&self->bpfProgram.bf_insns[3].k, &addr[2], 4);
        memcpy((void *)&self->bpfProgram.bf_insns[5].k, &addr, 2);

        return activateBpdFilter(self);
    }
    else
    {
        /* Disable Ethernet address filter. */
        self->bpfProgram.bf_insns[0].k = 0;

        return activateBpdFilter(self);
    }
}

static int
setBpfEthertypeFilter(EthernetSocket self, uint16_t etherType)
{
    if (etherType)
    {
        /* Enable Ethertype filter. */
        self->bpfProgram.bf_insns[6].k = 1;

        /* Set protocol. */
        self->bpfProgram.bf_insns[9].k = etherType;

        return activateBpdFilter(self);
    }
    else
    {
        /* Disable Ethertype filter. */
        self->bpfProgram.bf_insns[6].k = 0;

        return activateBpdFilter(self);
    }
}

void
Ethernet_getInterfaceMACAddress(const char* interfaceId, uint8_t* addr)
{
    struct ifaddrs *ifap, *ifc;
    struct sockaddr_dl* link;

    /* Get info about all local network interfaces. */
    if (getifaddrs(&ifap))
    {
        printf("Error getting network interfaces list!\n");
        return;
    }

    /* Try to find the selected interface. */
    ifc = ifap;
    while (ifc)
    {
        if (strcmp(ifc->ifa_name, interfaceId) == 0 &&
            ifc->ifa_addr->sa_family == AF_LINK && ifc->ifa_addr)
            break;
        ifc = ifc->ifa_next;
    }

    /* If we found the interface, extract MAC address from the info and copy to the destination. */
    if (ifc)
    {
        link = (struct sockaddr_dl *)ifc->ifa_addr;
        memcpy(addr, LLADDR(link), link->sdl_alen);
    }
    else
        printf("Could not find the network interface %s!\n", interfaceId);

    /* Free network interface info structure. */
    freeifaddrs(ifap);
}

EthernetSocket
Ethernet_createSocket(const char* interfaceId, uint8_t* destAddress)
{
    char bpfFileStringBuffer[11] = { 0 };
    int i;
    struct ifreq ifr;
    int optval;
    struct bpf_insn destAddrFiltCode[] =
    {
        /* Load 0 into accumulator. Change to 1 to enable ethernet address filter. */
        {0x00, 0, 0, 0x00000000},   /* A0:  ld #0 */
        {0x15, 4, 0, 0x00000000},   /*      jeq #0, P0, A1 */

        /* Load 4 bytes starting at offest 2 into the accu and compare it with 4 bytes of the destination address. */
        {0x20, 0, 0, 0x00000002},   /* A1:  ld [2] */
        {0x15, 0, 7, 0x00000000},   /*      jeq #0, A2, KO */

        /* Load 2 bytes starting at offest 0 into the accu and compare it with 2 bytes of the destination address. */
        {0x28, 0, 0, 0x00000000},   /* A2:  ldh [0] */
        {0x15, 0, 5, 0x00000000},   /*      jeq #0, P0, KO */

        /* Load 0 into accumulator. Change to 1 to enable ethernet protocol filter. */
        {0x00, 0, 0, 0x00000000},   /* P0:  ld #0 */
        {0x15, 2, 0, 0x00000000},   /*      jeq #0, OK, P1 */

        /* Load 2 bytes starting at offset 12 into the accu and compare it with the given ethertype. */
        {0x28, 0, 0, 0x0000000c},   /* P1:  ldh [12] */
        {0x15, 0, 1, 0x00000000},   /*      jeq #0, OK, KO */

        /* Accept packet. */
        {0x6, 0, 0, 0x0000ffff},    /* OK:  ret #65535 */

        /* Drop packet. */
        {0x6, 0, 0, 0x00000000}     /* KO:  ret #0 */

        /* The whole BPF VM assembler program compiled with bpfc into the machine code above:
         *
         * A0: ld #0
         *     jeq #0, P0, A1
         * A1: ld [2]
         *     jeq #0, A2, KO
         * A2: ldh [0]
         *     jeq #0, P0, KO
         * P0: ld #0
         *     jeq #0, OK, P1
         * P1: ldh [12]
         *     jeq #0, OK, KO
         * OK: ret #65535
         * KO: ret #0
         */
    };

    EthernetSocket self = GLOBAL_CALLOC(1, sizeof(struct sEthernetSocket));
    if (!self)
    {
        printf("Could not allocate socket descriptor!\n");
        return NULL;
    }

    /* Copy default BPF filter program into descriptor. */
    self->bpfProgram.bf_insns = GLOBAL_CALLOC(1, sizeof(destAddrFiltCode));
    if (!self->bpfProgram.bf_insns)
    {
        printf("Could not allocate memory for BPF filter program!\n");
        return NULL;
    }
    memcpy(self->bpfProgram.bf_insns, &destAddrFiltCode, sizeof(destAddrFiltCode));
    self->bpfProgram.bf_len = 12;

    /* Find the first unused BPF device node. */
    self->bpf = -1;
    for (i = 0; i < 99; ++i)
    {
        sprintf(bpfFileStringBuffer, "/dev/bpf%i", i);
        self->bpf = open(bpfFileStringBuffer, O_RDWR);

        if (self->bpf != -1) break;
    }

    /* Did not found any unused, fail. */
    if (self->bpf == -1)
    {
        printf("Error opening BPF file handle!\n");
        GLOBAL_FREEMEM(self->bpfProgram.bf_insns);
        GLOBAL_FREEMEM(self);
        return NULL;
    }

    /* Activate non-blocking operation. */
    optval = ioctl(self->bpf, F_GETFL);
    optval |= O_NONBLOCK;
    if (fcntl(self->bpf, F_SETFL, &optval) == -1)
    {
        printf("Unable to change to non-blocking mode!\n");
        GLOBAL_FREEMEM(self->bpfProgram.bf_insns);
        GLOBAL_FREEMEM(self);
        return NULL;
    }

    /* Select the network interface for the BPF. */
    strncpy(ifr.ifr_name, interfaceId, IFNAMSIZ);
    if (ioctl(self->bpf, BIOCSETIF, &ifr))
    {
        printf("Unable to select interface %s!\n", interfaceId);
        GLOBAL_FREEMEM(self->bpfProgram.bf_insns);
        GLOBAL_FREEMEM(self);
        return NULL;
    }

    /* Activate immediate mode. */
    optval = 1;
    if (ioctl(self->bpf, BIOCIMMEDIATE, &optval) == -1)
    {
        printf("Unable to activate immediate mode!\n");
        GLOBAL_FREEMEM(self->bpfProgram.bf_insns);
        GLOBAL_FREEMEM(self);
        return NULL;
    }

    /* Get the buffer length from the BPF handle. */
    if (ioctl(self->bpf, BIOCGBLEN, &self->bpfBufferSize) == -1)
    {
        printf("Unable to get BPF buffer lenght!\n");
        GLOBAL_FREEMEM(self->bpfProgram.bf_insns);
        GLOBAL_FREEMEM(self);
        return NULL;
    }

    /* Allocate a buffer for the message reception. */
    self->bpfBuffer = GLOBAL_CALLOC(1, self->bpfBufferSize);
    if (!self->bpfBuffer)
    {
        printf("Unable to allocate BPF RX buffer!\n");
        GLOBAL_FREEMEM(self->bpfProgram.bf_insns);
        GLOBAL_FREEMEM(self);
        return NULL;
    }
    self->bpfPositon = self->bpfBuffer;
    self->bpfEnd = self->bpfBuffer;

    /* Set BPF into promiscous mode. */
    optval = 1;
    if (ioctl(self->bpf, BIOCPROMISC, &optval) == -1)
    {
        printf("Unable to activate promiscous mode!\n");
        GLOBAL_FREEMEM(self->bpfProgram.bf_insns);
        GLOBAL_FREEMEM(self->bpfBuffer);
        GLOBAL_FREEMEM(self);
        return NULL;
    }

    return self;
}

void
Ethernet_setProtocolFilter(EthernetSocket self, uint16_t etherType)
{
    if (!self || !self->bpfProgram.bf_insns || setBpfEthertypeFilter(self, etherType))
        printf("Unable to set ethertype filter!\n");
}

int
Ethernet_receivePacket(EthernetSocket self, uint8_t* buffer, int bufferSize)
{
    /* If the actual buffer is empty, make a read call to the BSP device in order to get new data. */
    if (self->bpfEnd - self->bpfPositon < 4)
    {
        /* Position the read pointer to the start of the buffer. */
        self->bpfPositon = self->bpfBuffer;

        /* Read one or more frames from the BPF handle. */
        int size = read(self->bpf, self->bpfBuffer, self->bpfBufferSize);

        /* Set the end pointer to the end of the received data or to 0 if no data at all was received. */
        if (size >= 0)
            self->bpfEnd = self->bpfBuffer + size;
        else
            self->bpfEnd = NULL;
    }

    /* Do we actually have at least one ethernet frame received? */
    if (self->bpfPositon < self->bpfEnd)
    {
        /* BPF adds a header to each packet, so we have to interpret it. */
        struct bpf_hdr *header = (struct bpf_hdr *)(self->bpfPositon);

        /* Check if the target buffer is big enough to hold the received ethernet frame. */
        if ((unsigned int) bufferSize >= header->bh_caplen)
        {
            /* Copy the frame to the target buffer. */
            memcpy(buffer, self->bpfPositon + header->bh_hdrlen, header->bh_caplen);

            /* Move the read pointer to the next ethernet frame header WORD ALIGNED (Took me a while to find that out). */
            self->bpfPositon += BPF_WORDALIGN(header->bh_hdrlen + header->bh_caplen);

            /* Return the number of bytes copied to the target buffer. */
            return header->bh_caplen;
        }
        else
            /* The buffer is too small, return an error. */
            /* TODO: Would be there a standard error number to signal that the target buffer is too small? */
            return -1;
    }
    else
        /* We did not get any ethernet frames, so return 0. */
        return 0;
}

void
Ethernet_sendPacket(EthernetSocket self, uint8_t* buffer, int packetSize)
{
    /* Just send the packet as it is. */
    write(self->bpf, buffer, packetSize);
}

void
Ethernet_destroySocket(EthernetSocket self)
{
    /* Close the BPF device. */
    close(self->bpf);

    /* Free all dynamic resources used by the ethernet socket. */
    GLOBAL_FREEMEM(self->bpfBuffer);
    GLOBAL_FREEMEM(self->bpfProgram.bf_insns);
    GLOBAL_FREEMEM(self);
}

bool
Ethernet_isSupported()
{
    return true;
}

