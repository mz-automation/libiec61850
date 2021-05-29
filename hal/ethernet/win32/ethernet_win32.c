/*
 *  ethernet_win32.c
 *
 *  Copyright 2013 Michael Zillgith
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

#include "stack_config.h"

#include <string.h>
#include <stdio.h>

#include "hal_ethernet.h"
#include "lib_memory.h"

#ifndef DEBUG_HAL_ETHERNET
#define DEBUG_HAL_ETHERNET 1
#endif

// Set to 1 to workaround WaitForMutlipleObjects problem (returns timeout even when packets are received)
#define ETHERNET_WIN32_DISABLE_ETHERNET_HANDLESET 1

#if (CONFIG_INCLUDE_ETHERNET_WINDOWS == 1)


#include <malloc.h>

#include <winsock2.h>
#include <iphlpapi.h>

#ifndef __GNUC__
#pragma comment (lib, "IPHLPAPI.lib")
#endif

#define HAVE_REMOTE

// Enable WinPcap specific extension: pcap_getevent()
#define WPCAP
#include "pcap.h"

struct sEthernetSocket {
    pcap_t* rawSocket;
    struct bpf_program etherTypeFilter;
};

struct sEthernetHandleSet {
    HANDLE *handles;
    int nhandles;
};

#ifdef __GNUC__ /* detect MINGW */

#ifndef __MINGW64_VERSION_MAJOR

#define MAX_ADAPTER_ADDRESS_LENGTH      8

typedef struct _IP_ADAPTER_ADDRESSES {
	union {
		ULONGLONG Alignment;
		struct {
			ULONG Length;
			DWORD IfIndex;
		};
	};
	struct _IP_ADAPTER_ADDRESSES *Next;
	PCHAR AdapterName;
	void* FirstUnicastAddress;
	void* FirstAnycastAddress;
	void* FirstMulticastAddress;
	void* FirstDnsServerAddress;
	PWCHAR DnsSuffix;
	PWCHAR Description;
	PWCHAR FriendlyName;
	BYTE PhysicalAddress[MAX_ADAPTER_ADDRESS_LENGTH];
	DWORD PhysicalAddressLength;
	DWORD Flags;
	DWORD Mtu;
	DWORD IfType;
} IP_ADAPTER_ADDRESSES, *PIP_ADAPTER_ADDRESSES;


typedef ULONG (WINAPI* pgetadaptersaddresses)(ULONG family, ULONG flags, PVOID reserved, PIP_ADAPTER_ADDRESSES AdapterAddresses,
		PULONG SizePointer);

static pgetadaptersaddresses GetAdaptersAddresses;

static bool dllLoaded = false;

static void
loadDLLs(void)
{
    HINSTANCE hDll = LoadLibrary("iphlpapi.dll");

    if (hDll == NULL) {
        if (DEBUG_HAL_ETHERNET)
            printf("Error loading iphlpapi.dll!\n");
        return;
    }

    GetAdaptersAddresses = (pgetadaptersaddresses) GetProcAddress(hDll,
            "GetAdaptersAddresses");

    if (GetAdaptersAddresses == NULL)
        printf("Error loading GetAdaptersAddresses from iphlpapi.dll (%d)\n", (int) GetLastError());
}

#endif /* __MINGW64_VERSION_MAJOR */

#endif /* __GNUC__ */

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
#if ETHERNET_WIN32_DISABLE_ETHERNET_HANDLESET == 1
#else
    if (self != NULL && sock != NULL) {

        int i = self->nhandles++;

        self->handles = (HANDLE *) realloc(self->handles, self->nhandles * sizeof(HANDLE));
            
        self->handles[i] = pcap_getevent(sock->rawSocket);
    }
#endif
}

void
EthernetHandleSet_removeSocket(EthernetHandleSet self, const EthernetSocket sock)
{
#if ETHERNET_WIN32_DISABLE_ETHERNET_HANDLESET == 1
#else
    if ((self != NULL) && (sock != NULL)) {
        HANDLE h = pcap_getevent(sock->rawSocket);

        int i;

        for (i = 0; i < self->nhandles; i++) {
            if (self->handles[i] == h) {
                memmove(&self->handles[i], &self->handles[i+1], sizeof(HANDLE) * (self->nhandles - i - 1));
                self->nhandles--;
                return;
            }
        }
    }
#endif
}

int
EthernetHandleSet_waitReady(EthernetHandleSet self, unsigned int timeoutMs)
{
#if ETHERNET_WIN32_DISABLE_ETHERNET_HANDLESET == 1
	return 1;
#else
    int result;

    if ((self != NULL) && (self->nhandles > 0)) {
        DWORD ret = WaitForMultipleObjects(self->nhandles, self->handles, 0, timeoutMs);

        if (ret == WAIT_TIMEOUT)
            result = 0;
        else if (ret == WAIT_FAILED)
            result = -1;
        else
            result = (int)ret;
    }
    else {
       result = -1;
    }

    return result;
#endif
}

void
EthernetHandleSet_destroy(EthernetHandleSet self)
{
    if (self->handles)
        free(self->handles);

    GLOBAL_FREEMEM(self);
}

static char*
getInterfaceName(int interfaceIndex)
{
    char errbuf[PCAP_ERRBUF_SIZE];
    char* interfaceName = NULL;

    pcap_if_t *devices;
    pcap_if_t *device;

    /* Get the ethernet device list */
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &devices, errbuf) == -1)
    {
        printf("pcap_findalldevs_ex: %s\n", errbuf);
        return NULL;
    }

    bool ifaceFound = false;

    /* Search device list for requested interface) */
    int i = 0;
    for(device = devices; device != NULL; device= device->next)
    {
        if (i == interfaceIndex) {
            interfaceName = (char*) malloc(strlen(device->name) + 1);
            strcpy(interfaceName, device->name);
            if (DEBUG_HAL_ETHERNET)
                printf("Use interface (%s)\n", interfaceName);
            ifaceFound = true;
            break;
        }

        i++;
    }

    if (!ifaceFound)
    {
        if (DEBUG_HAL_ETHERNET)
            printf("No ethernet interfaces found! Make sure WinPcap is installed.\n");
        return NULL;
    }

    pcap_freealldevs(devices);

    return interfaceName;
}

static void
getAdapterMacAddress(char* pcapAdapterName, uint8_t* macAddress)
{
    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    ULONG outBufLen = 0;

    pAddresses = (IP_ADAPTER_ADDRESSES*) malloc(65000);

    if (GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAddresses);
        pAddresses = (IP_ADAPTER_ADDRESSES*) malloc(outBufLen);
    }

    if (GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen) == NO_ERROR) {
        PIP_ADAPTER_ADDRESSES pAddress = pAddresses;

        while (pAddress != NULL) {

            DWORD addressLength = pAddress->PhysicalAddressLength;

            if (addressLength == 6) {

                int i;

                if (DEBUG_HAL_ETHERNET) {
                    printf("Adapter %s: ", pAddress->AdapterName);

                    for (i = 0; i < (int) addressLength; i++) {
                        printf("%02x ", pAddress->PhysicalAddress[i]);
                    }
                }

                if (strstr(pcapAdapterName, pAddress->AdapterName) != 0) {
                    if (DEBUG_HAL_ETHERNET)
                        printf(" requested found!");

                    for (i = 0; i < (int) addressLength; i++) {
                        macAddress[i] = pAddress->PhysicalAddress[i];
                    }
                }

                printf("\n");
            }

            pAddress = pAddress->Next;
        }

        free(pAddresses);
    }
    else {
        printf("Error getting device addresses!\n");
    }
}

void
Ethernet_getInterfaceMACAddress(const char* interfaceId, uint8_t* addr)
{
#ifdef __GNUC__
#ifndef __MINGW64_VERSION_MAJOR
    if (!dllLoaded) {
        loadDLLs();
        dllLoaded = true;
    }
#endif
#endif

    char* endPtr;

    long interfaceIndex = strtol(interfaceId, &endPtr, 10);

    if ((*interfaceId != '\0') && (*endPtr != '\0')) {
        printf("Ethernet_getInterfaceMACAddress: invalid interface number %s\n", interfaceId);
        return;
    }

    char* interfaceName = getInterfaceName((int) interfaceIndex);

    getAdapterMacAddress(interfaceName, addr);

    free(interfaceName);
}


EthernetSocket
Ethernet_createSocket(const char* interfaceId, uint8_t* destAddress)
{
    pcap_t *pcapSocket;
    char errbuf[PCAP_ERRBUF_SIZE];

    int interfaceIndex = atoi(interfaceId);

    char* interfaceName = getInterfaceName(interfaceIndex);

    if ((pcapSocket = pcap_open_live(interfaceName, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1, errbuf)) == NULL)
    {
        printf("Open ethernet socket failed for device %s\n", interfaceName);
        free(interfaceName);
        return NULL;
    }

    free(interfaceName);

    EthernetSocket ethernetSocket = (EthernetSocket) calloc(1, sizeof(struct sEthernetSocket));

    ethernetSocket->rawSocket = pcapSocket;

    return ethernetSocket;
}

void
Ethernet_destroySocket(EthernetSocket ethSocket)
{
    pcap_close(ethSocket->rawSocket);
    free(ethSocket);
}

void
Ethernet_sendPacket(EthernetSocket ethSocket, uint8_t* buffer, int packetSize)
{
    if (pcap_sendpacket(ethSocket->rawSocket, buffer, packetSize) != 0)
        printf("Error sending the packet: %s\n", pcap_geterr(ethSocket->rawSocket));
}

void
Ethernet_setProtocolFilter(EthernetSocket ethSocket, uint16_t etherType)
{
    char filterString[100];

    sprintf(filterString, "(ether proto 0x%04x) or (vlan and ether proto 0x%04x)", etherType, etherType);

    if (pcap_compile(ethSocket->rawSocket, &(ethSocket->etherTypeFilter), filterString, 1, 0) < 0) {
        printf("Compiling packet filter failed!\n");
        return;
    }

    if (pcap_setfilter(ethSocket->rawSocket, &(ethSocket->etherTypeFilter)) < 0) {
        printf("Setting packet filter failed!\n");
    }
}

int
Ethernet_receivePacket(EthernetSocket self, uint8_t* buffer, int bufferSize)
{
    struct pcap_pkthdr* header;
    uint8_t* packetData;

    int pcapCode = pcap_next_ex(self->rawSocket, &header, (const unsigned char**) &packetData);

    if (pcapCode > 0) {
        int packetSize = header->caplen;

        if (packetSize > bufferSize)
        packetSize = bufferSize;

        memcpy(buffer, packetData, packetSize);

        return packetSize;
    }
    else {
        if (pcapCode < 0)
            printf("winpcap error\n");

        return 0;
    }
}

bool
Ethernet_isSupported()
{
    return true;
}

#else

bool
Ethernet_isSupported()
{
    return false;
}

EthernetHandleSet
EthernetHandleSet_new(void)
{
    return NULL;
}

void
EthernetHandleSet_addSocket(EthernetHandleSet self, const EthernetSocket sock)
{
}

int
EthernetHandleSet_waitReady(EthernetHandleSet self, unsigned int timeoutMs)
{
    return 0;
}

void
EthernetHandleSet_destroy(EthernetHandleSet self)
{
}

void
Ethernet_getInterfaceMACAddress(const char* interfaceId, uint8_t* addr)
{
}

EthernetSocket
Ethernet_createSocket(const char* interfaceId, uint8_t* destAddress)
{
    return NULL;
}

void
Ethernet_destroySocket(EthernetSocket ethSocket)
{
}

void
Ethernet_sendPacket(EthernetSocket ethSocket, uint8_t* buffer, int packetSize)
{
}

void
Ethernet_setProtocolFilter(EthernetSocket ethSocket, uint16_t etherType)
{
}

int
Ethernet_receivePacket(EthernetSocket self, uint8_t* buffer, int bufferSize)
{
    return 0;
}

#endif /* (CONFIG_INCLUDE_ETHERNET_WINDOWS == 1) */
