/*
 *  thread_hal.h
 *
 *  Multi-threading abstraction layer
 *
 *  Copyright 2013-2023 Michael Zillgith
 *
 *  This file is part of Platform Abstraction Layer (libpal)
 *  for libiec61850, libmms, and lib60870.
 */

#ifndef LIBGCOS_THREAD_HAL_H_
#define LIBGCOS_THREAD_HAL_H_

#include "hal_base.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CONFIG_LIBGCOS_LEGACY_NAMES
#define CONFIG_LIBGCOS_LEGACY_NAMES 1
#endif

#ifndef CONFIG_LIBGCOS_PREFIX
#define CONFIG_LIBGCOS_PREFIX 0
#endif

#if (CONFIG_LIBGCOS_PREFIX == 1)

#define THREAD_CREATE libgcos_Thread_create
#define THREAD_START libgcos_Thread_start
#define THREAD_DESTROY libgcos_Thread_destroy
#define THREAD_SLEEP libgcos_Thread_sleep
#define SEMAPHORE_CREATE libgcos_Semaphore_create
#define SEMAPHORE_WAIT libgcos_Semaphore_wait
#define SEMAPHORE_POST libgcos_Semaphore_post
#define SEMAPHORE_DESTROY libgcos_Semaphore_destroy

#if (CONFIG_LIBGCOS_LEGACY_NAMES == 1) 

#define Thread_create THREAD_CREATE
#define Thread_start THREAD_START
#define Thread_destroy THREAD_DESTROY
#define Thread_sleep THREAD_SLEEP
#define Semaphore_create SEMAPHORE_CREATE
#define Semaphore_wait SEMAPHORE_WAIT
#define Semaphore_post SEMAPHORE_POST
#define Semaphore_destroy SEMAPHORE_DESTROY

#endif /* (CONFIG_LIBGCOS_LEGACY_NAMES == 1)  */

#else

#define THREAD_CREATE Thread_create
#define THREAD_START Thread_start
#define THREAD_DESTROY Thread_destroy
#define THREAD_SLEEP Thread_sleep
#define SEMAPHORE_CREATE Semaphore_create
#define SEMAPHORE_WAIT Semaphore_wait
#define SEMAPHORE_POST Semaphore_post
#define SEMAPHORE_DESTROY Semaphore_destroy

#endif /* (CONFIG_LIBGCOS_PREFIX == 1) */

/**
 * \file hal_thread.h
 * \brief Abstraction layer for threading and synchronization
 */

/*! \addtogroup hal
   *
   *  @{
   */

/**
 * @defgroup HAL_THREAD Threading and synchronization API
 *
 * @{
 */

/** Opaque reference of a Thread instance */
typedef struct sThread* Thread;

/** Qpaque reference of a Semaphore instance */
typedef void* Semaphore;

/** Reference to a function that is called when starting the thread */
typedef void* (*ThreadExecutionFunction) (void*);

/**
 * \brief Create a new Thread instance
 *
 * \param function the entry point of the thread
 * \param parameter a parameter that is passed to the threads start function
 * \param autodestroy the thread is automatically destroyed if the ThreadExecutionFunction has finished.
 *
 * \return the newly created Thread instance
 */
PAL_API Thread
THREAD_CREATE(ThreadExecutionFunction function, void* parameter, bool autodestroy);

/**
 * \brief Start a Thread.
 *
 * This function invokes the start function of the thread. The thread terminates when
 * the start function returns.
 *
 * \param thread the Thread instance to start
 */
PAL_API void
THREAD_START(Thread thread);

/**
 * \brief Destroy a Thread and free all related resources.
 *
 * \param thread the Thread instance to destroy
 */
PAL_API void
THREAD_DESTROY(Thread thread);

/**
 * \brief Suspend execution of the Thread for the specified number of milliseconds
 */
PAL_API void
THREAD_SLEEP(int millies);

PAL_API Semaphore
SEMAPHORE_CREATE(int initialValue);

/* Wait until semaphore value is greater than zero. Then decrease the semaphore value. */
PAL_API void
SEMAPHORE_WAIT(Semaphore self);

PAL_API void
SEMAPHORE_POST(Semaphore self);

PAL_API void
SEMAPHORE_DESTROY(Semaphore self);

/*! @} */

/*! @} */

#ifdef __cplusplus
}
#endif

#endif /* LIBGCOS_THREAD_HAL_H_ */
