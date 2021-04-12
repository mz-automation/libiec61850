/**
 * thread_macos.c
 *
 * Copyright 2013-2019 MZ Automation GmbH
 *
 * This file is part of Platform Abstraction Layer (libpal)
 * for libiec61850, libmms, and lib60870.
 */

/*
 * NOTE: MacOS needs own thread layer because it doesn't support unnamed semaphores!
 */

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include "hal_thread.h"
#include "lib_memory.h"

struct sThread {
   ThreadExecutionFunction function;
   void* parameter;
   pthread_t pthread;
   int state;
   bool autodestroy;
};

typedef struct sSemaphore* mSemaphore;

struct sSemaphore
{
    sem_t* sem;
};

Semaphore
Semaphore_create(int initialValue)
{
    mSemaphore self = NULL;

    char tmpname[] = {"/tmp/libiec61850.XXXXXX"};
    char* res = mktemp(tmpname);

    if (res) {
        self = (mSemaphore) GLOBAL_CALLOC(1, sizeof(struct sSemaphore));

        if (self) {
            self->sem = sem_open(tmpname, O_CREAT, 0666, initialValue);

            if (self->sem == SEM_FAILED) {
                printf("ERROR: Failed to create semaphore (errno = %i)\n", errno);

                GLOBAL_FREEMEM(self);
                self = NULL;
            }
            else {
                int ret = sem_unlink(tmpname);

                if (ret == -1)
                    printf("ERROR: Failed to unlink semaphore %s\n", tmpname);
            }
        }
    }

    return (Semaphore)self;
}

/* Wait until semaphore value is more than zero. Then decrease the semaphore value. */
void
Semaphore_wait(Semaphore self)
{
    mSemaphore mSelf = (mSemaphore) self;

    sem_wait(mSelf->sem);
}

void
Semaphore_post(Semaphore self)
{
    mSemaphore mSelf = (mSemaphore) self;

    sem_post(mSelf->sem);
}

void
Semaphore_destroy(Semaphore self)
{
    if (self) {
        mSemaphore mSelf = (mSemaphore) self;

        int ret = sem_close(mSelf->sem);

        if (ret == -1)
            printf("ERROR: Failed to close semaphore (errno = %i)\n", errno);

        GLOBAL_FREEMEM(mSelf);
    }    
}

Thread
Thread_create(ThreadExecutionFunction function, void* parameter, bool autodestroy)
{
   Thread thread = (Thread) GLOBAL_MALLOC(sizeof(struct sThread));

   if (thread != NULL) {
        thread->parameter = parameter;
        thread->function = function;
        thread->state = 0;
        thread->autodestroy = autodestroy;
   }

   return thread;
}

static void*
destroyAutomaticThread(void* parameter)
{
    Thread thread = (Thread) parameter;

    thread->function(thread->parameter);

    GLOBAL_FREEMEM(thread);

    pthread_exit(NULL);
}

void
Thread_start(Thread thread)
{
   if (thread->autodestroy == true) {
       pthread_create(&thread->pthread, NULL, destroyAutomaticThread, thread);
       pthread_detach(thread->pthread);
   }
   else
       pthread_create(&thread->pthread, NULL, thread->function, thread->parameter);

   thread->state = 1;
}

void
Thread_destroy(Thread thread)
{
   if (thread->state == 1) {
       pthread_join(thread->pthread, NULL);
   }

   GLOBAL_FREEMEM(thread);
}

void
Thread_sleep(int millies)
{
   usleep(millies * 1000);
}
