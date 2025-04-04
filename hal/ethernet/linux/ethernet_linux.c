/*
 *  ethernet_linux.c
 *
 *  Copyright 2013-2024 Michael Zillgith
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
#include <poll.h>
#include <linux/filter.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

#include <string.h>

#include "lib_memory.h"
#include "hal_ethernet.h"

#ifndef DEBUG_SOCKET
#define DEBUG_SOCKET 0
#endif

struct sEthernetSocket {
    int rawSocket;
    bool isBind;
    struct sockaddr_ll socketAddress;
};

struct sEthernetHandleSet {
    struct pollfd* handles;
    int nhandles;
};

EthernetHandleSet
EthernetHandleSet_new(void)
{
    EthernetHandleSet result = (EthernetHandleSet) GLOBAL_MALLOC(sizeof(struct sEthernetHandleSet));

    if (result != NULL)
    {
        result->handles = NULL;
        result->nhandles = 0;
    }

    return result;
}

void
EthernetHandleSet_addSocket(EthernetHandleSet self, const EthernetSocket sock)
{
    if (self != NULL && sock != NULL)
    {
        int i = self->nhandles++;

        self->handles = realloc(self->handles, self->nhandles * sizeof(struct pollfd));

        self->handles[i].fd = sock->rawSocket;
        self->handles[i].events = POLLIN;
    }
}

void
EthernetHandleSet_removeSocket(EthernetHandleSet self, const EthernetSocket sock)
{
    if ((self != NULL) && (sock != NULL))
    {
        int i;

        for (i = 0; i < self->nhandles; i++)
        {
            if (self->handles[i].fd == sock->rawSocket)
            {
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

static int
getInterfaceIndex(int sock, const char* deviceName)
{
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(struct ifreq));

    strncpy(ifr.ifr_name, deviceName, IFNAMSIZ - 1);

    if (ioctl(sock, SIOCGIFINDEX, &ifr) == -1)
    {
        if (DEBUG_SOCKET)
            printf("ETHERNET_LINUX: Failed to get interface index");
        return -1;
    }

    int interfaceIndex = ifr.ifr_ifindex;

    return interfaceIndex;
}


void
Ethernet_getInterfaceMACAddress(const char* interfaceId, uint8_t* addr)
{
    struct ifreq buffer;

    int sock = socket(PF_INET, SOCK_DGRAM, 0);

    memset(&buffer, 0x00, sizeof(buffer));

    strncpy(buffer.ifr_name, interfaceId, IFNAMSIZ - 1);

    ioctl(sock, SIOCGIFHWADDR, &buffer);

    close(sock);

    int i;

    for (i = 0; i < 6; i++ )
    {
        addr[i] = (unsigned char)buffer.ifr_hwaddr.sa_data[i];
    }
}

EthernetSocket
Ethernet_createSocket(const char* interfaceId, uint8_t* destAddress)
{
    EthernetSocket self = GLOBAL_CALLOC(1, sizeof(struct sEthernetSocket));

    if (self)
    {
        self->rawSocket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

        if (self->rawSocket == -1)
        {
            if (DEBUG_SOCKET)
                printf("Error creating raw socket!\n");
            GLOBAL_FREEMEM(self);
            return NULL;
        }

        self->socketAddress.sll_family = PF_PACKET;
        self->socketAddress.sll_protocol = htons(ETH_P_ALL);

        int ifcIdx =  getInterfaceIndex(self->rawSocket, interfaceId);

        if (ifcIdx == -1)
        {
            Ethernet_destroySocket(self);
            return NULL;
        }

        self->socketAddress.sll_ifindex = ifcIdx;

        self->socketAddress.sll_hatype =  ARPHRD_ETHER;
        self->socketAddress.sll_pkttype = PACKET_HOST | PACKET_MULTICAST;

        self->socketAddress.sll_halen = ETH_ALEN;

        memset(self->socketAddress.sll_addr, 0, 8);

        if (destAddress != NULL)
            memcpy(self->socketAddress.sll_addr, destAddress, 6);

        self->isBind = false;

        Ethernet_setMode(self, ETHERNET_SOCKET_MODE_PROMISC);
    }

    return self;
}

void
Ethernet_setMode(EthernetSocket self, EthernetSocketMode mode)
{
    if (self)
    {
        if (mode == ETHERNET_SOCKET_MODE_PROMISC)
        {
            struct ifreq ifr;

            if (ioctl (self->rawSocket, SIOCGIFFLAGS, &ifr) == -1)
            {
                if (DEBUG_SOCKET)
                    printf("ETHERNET_LINUX: Problem getting device flags");
                return;
            }

            ifr.ifr_flags |= IFF_PROMISC;
            if (ioctl (self->rawSocket, SIOCSIFFLAGS, &ifr) == -1)
            {
                if (DEBUG_SOCKET)
                    printf("ETHERNET_LINUX: Setting device to promiscuous mode failed");
                return;
            }
        }
        else if (mode == ETHERNET_SOCKET_MODE_ALL_MULTICAST)
        {
            struct ifreq ifr;

            if (ioctl (self->rawSocket, SIOCGIFFLAGS, &ifr) == -1)
            {
                if (DEBUG_SOCKET)
                    printf("ETHERNET_LINUX: Problem getting device flags");
                return;
            }

            ifr.ifr_flags |= IFF_ALLMULTI;
            if (ioctl (self->rawSocket, SIOCSIFFLAGS, &ifr) == -1)
            {
                if (DEBUG_SOCKET)
                    printf("ETHERNET_LINUX: Setting device to promiscuous mode failed");
                return;
            }
        }
        else if (mode == ETHERNET_SOCKET_MODE_HOST_ONLY)
        {
            self->socketAddress.sll_pkttype = PACKET_HOST;
        }
        else if (mode == ETHERNET_SOCKET_MODE_MULTICAST)
        {
            self->socketAddress.sll_pkttype = PACKET_HOST | PACKET_MULTICAST;
        }
    }
}

void
Ethernet_addMulticastAddress(EthernetSocket self, uint8_t* multicastAddress)
{
    struct packet_mreq mreq;
    memset(&mreq, 0, sizeof(struct packet_mreq));

    mreq.mr_ifindex = self->socketAddress.sll_ifindex;
    mreq.mr_alen = ETH_ALEN;
    mreq.mr_type = PACKET_MR_MULTICAST;
    mreq.mr_address[0] = multicastAddress[0];
    mreq.mr_address[1] = multicastAddress[1];
    mreq.mr_address[2] = multicastAddress[2];
    mreq.mr_address[3] = multicastAddress[3];
    mreq.mr_address[4] = multicastAddress[4];
    mreq.mr_address[5] = multicastAddress[5];

    int res = setsockopt(self->rawSocket, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mreq, sizeof(mreq));

    if (res != 0)
    {
        if (DEBUG_SOCKET)
            printf("ETHERNET_LINUX: Setting multicast address failed");
    }
}

void
Ethernet_setProtocolFilter(EthernetSocket self, uint16_t etherType)
{
    if (etherType == 0x88b8)
    {
        /* enable linux kernel filtering for GOOSE */

        struct sock_fprog fprog;
        struct sock_filter filter[] = {
            /* sudo tcpdump -i enx88d7f6377223 '(ether proto 0x88b8)' -dd */
            { 0x28, 0, 0, 0x0000000c },
            { 0x15, 0, 1, 0x000088b8 },
            { 0x6, 0, 0, 0x00040000 },
            { 0x6, 0, 0, 0x00000000 }
        };

        fprog.len = sizeof(filter) / sizeof(*filter);
        fprog.filter = filter;

        if (setsockopt(self->rawSocket, SOL_SOCKET, SO_ATTACH_FILTER, &fprog, sizeof(fprog)) == -1)
            if (DEBUG_SOCKET)
                printf("ETHERNET_LINUX: Applying filter failed");
    }
    else
    {
        self->socketAddress.sll_protocol = htons(etherType);
    }
}

/* non-blocking receive */
int
Ethernet_receivePacket(EthernetSocket self, uint8_t* buffer, int bufferSize)
{
    if (self->isBind == false)
    {
        if (bind(self->rawSocket, (struct sockaddr*) &self->socketAddress, sizeof(self->socketAddress)) == 0)
            self->isBind = true;
        else
            return 0;
    }

    return recvfrom(self->rawSocket, buffer, bufferSize, MSG_DONTWAIT, 0, 0);
}

void
Ethernet_sendPacket(EthernetSocket self, uint8_t* buffer, int packetSize)
{
    sendto(self->rawSocket, buffer, packetSize,
                0, (struct sockaddr*) &(self->socketAddress), sizeof(self->socketAddress));
}

void
Ethernet_destroySocket(EthernetSocket self)
{
    if (self)
    {
        close(self->rawSocket);
        GLOBAL_FREEMEM(self);
    }
}

bool
Ethernet_isSupported()
{
    return true;
}
