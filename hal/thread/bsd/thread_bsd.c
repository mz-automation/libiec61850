/**
 * thread_bsd.c
 *
 * Copyright 2016 Michael Zillgith
 *
 * This file is part of libIEC61850.
 *
 * libIEC61850 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libIEC61850 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 * See COPYING file for the complete license text.
 */

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "hal_thread.h"

#include "lib_memory.h"

struct sThread {
   ThreadExecutionFunction function;
   void* parameter;
   pthread_t pthread;
   int state;
   bool autodestroy;
};

Semaphore
Semaphore_create(int initialValue)
{
    Semaphore self = GLOBAL_MALLOC(sizeof(sem_t));

    sem_init((sem_t*) self, 0, initialValue);

    return self;
}

/* Wait until semaphore value is more than zero. Then decrease the semaphore value. */
void
Semaphore_wait(Semaphore self)
{
    sem_wait((sem_t*) self);
}

void
Semaphore_post(Semaphore self)
{
    sem_post((sem_t*) self);
}

void
Semaphore_destroy(Semaphore self)
{
    sem_close(self);
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
