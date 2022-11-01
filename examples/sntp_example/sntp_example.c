/*
 * sntp_example.c
 */

#include "sntp_client.h"
#include "hal_thread.h"
#include <signal.h>
#include <stdio.h>

static bool running = true;

void
sigint_handler(int signalId)
{
    running = false;
}

static void
sntpUserCallback(void* parameter, bool isSynced)
{
    if (isSynced)
        printf("SNTP: Clock synchronized\n");
    else
        printf("SNTP: Clock not synchronized\n");
}

int
main(int argc, char** argv)
{
    SNTPClient client = SNTPClient_create();

    SNTPClient_addServer(client, "192.168.178.74", 123);

    SNTPClient_setUserCallback(client, sntpUserCallback, NULL);

    SNTPClient_setPollInterval(client, 16);

    SNTPClient_start(client);

    signal(SIGINT, sigint_handler);

    while (running) {
        Thread_sleep(100);
    }

    SNTPClient_destroy(client);
}

