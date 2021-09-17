/*
 *  thread_win32.c
 *
 *  Copyright 2013-2021 Michael Zillgith
 *
 *  This file is part of Platform Abstraction Layer (libpal)
 *  for libiec61850, libmms, and lib60870.
 */

#include <windows.h>
#include "lib_memory.h"
#include "hal_thread.h"

struct sThread {
	ThreadExecutionFunction function;
	void* parameter;
	HANDLE handle;
	int state;
	bool autodestroy;
};

static DWORD WINAPI
destroyAutomaticThreadRunner(LPVOID parameter)
{
	Thread thread = (Thread) parameter;

	thread->function(thread->parameter);

	thread->state = 0;

	Thread_destroy(thread);

	return 0;
}

static DWORD WINAPI
threadRunner(LPVOID parameter)
{
	Thread thread = (Thread) parameter;

	return (UINT) thread->function(thread->parameter);
}

Thread
Thread_create(ThreadExecutionFunction function, void* parameter, bool autodestroy)
{
	DWORD threadId;
	Thread thread = (Thread) GLOBAL_MALLOC(sizeof(struct sThread));

	thread->parameter = parameter;
	thread->function = function;
	thread->state = 0;
	thread->autodestroy = autodestroy;

	if (autodestroy == true)
		thread->handle = CreateThread(0, 0, destroyAutomaticThreadRunner, thread, CREATE_SUSPENDED, &threadId);
	else
		thread->handle = CreateThread(0, 0, threadRunner, thread, CREATE_SUSPENDED, &threadId);

	return thread;
}

void
Thread_start(Thread thread)
{
	thread->state = 1;
	ResumeThread(thread->handle);
}

void
Thread_destroy(Thread thread)
{
	if (thread->state == 1)
		WaitForSingleObject(thread->handle, INFINITE);

	CloseHandle(thread->handle);

	GLOBAL_FREEMEM(thread);
}

void
Thread_sleep(int millies)
{
	Sleep(millies);
}

Semaphore
Semaphore_create(int initialValue)
{
    HANDLE self = CreateSemaphore(NULL, 1, 1, NULL);

    return self;
}

/* Wait until semaphore value is greater than zero. Then decrease the semaphore value. */
void
Semaphore_wait(Semaphore self)
{
    WaitForSingleObject((HANDLE) self, INFINITE);
}

void
Semaphore_post(Semaphore self)
{
    ReleaseSemaphore((HANDLE) self, 1, NULL);
}

void
Semaphore_destroy(Semaphore self)
{
    CloseHandle((HANDLE) self);
}
