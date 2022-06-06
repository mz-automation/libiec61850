/**
 * thread_macos.c
 *
 * Copyright 2013-2021 Michael Zillgith
 *
 * This file is part of Platform Abstraction Layer (libpal)
 * for libiec61850, libmms, and lib60870.
 */

/*
 * NOTE: MacOS needs own thread layer because it doesn't support unnamed semaphores!
 * NOTE: named semaphores were replaced by POSIX mutex
 */

#include <pthread.h>
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
    pthread_mutex_t mutex;
};

/*
 * NOTE: initialValue is ignored because semaphore was replaced by mutex
 */
Semaphore
Semaphore_create(int initialValue)
{
    mSemaphore self = NULL;

    self = (mSemaphore) GLOBAL_CALLOC(1, sizeof(struct sSemaphore));

    if (self) {
        pthread_mutex_init(&(self->mutex), NULL);
    }

    return (Semaphore)self;
}

/* lock mutex */
void
Semaphore_wait(Semaphore self)
{
    mSemaphore mSelf = (mSemaphore) self;

    int retVal = pthread_mutex_lock(&(mSelf->mutex));

    if (retVal) {
       printf("FATAL ERROR: pthread_mutex_lock failed (err=%i)\n", retVal);
       exit(-1);
    }
}

/* unlock mutex */
void
Semaphore_post(Semaphore self)
{
    mSemaphore mSelf = (mSemaphore) self;

    int retVal = pthread_mutex_unlock(&(mSelf->mutex));

    if (retVal) {
        printf("FATAL ERROR: pthread_mutex_unlock failed (err=%i)\n", retVal);
        exit(-1);
    }
}

void
Semaphore_destroy(Semaphore self)
{
    if (self) {
        mSemaphore mSelf = (mSemaphore) self;

        pthread_mutex_destroy(&(mSelf->mutex));

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
