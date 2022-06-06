/*
 *  thread_hal.h
 *
 *  Multi-threading abstraction layer
 *
 *  Copyright 2013-2021 Michael Zillgith
 *
 *  This file is part of Platform Abstraction Layer (libpal)
 *  for libiec61850, libmms, and lib60870.
 */

#ifndef THREAD_HAL_H_
#define THREAD_HAL_H_

#include "hal_base.h"

#ifdef __cplusplus
extern "C" {
#endif

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
Thread_create(ThreadExecutionFunction function, void* parameter, bool autodestroy);

/**
 * \brief Start a Thread.
 *
 * This function invokes the start function of the thread. The thread terminates when
 * the start function returns.
 *
 * \param thread the Thread instance to start
 */
PAL_API void
Thread_start(Thread thread);

/**
 * \brief Destroy a Thread and free all related resources.
 *
 * \param thread the Thread instance to destroy
 */
PAL_API void
Thread_destroy(Thread thread);

/**
 * \brief Suspend execution of the Thread for the specified number of milliseconds
 */
PAL_API void
Thread_sleep(int millies);

PAL_API Semaphore
Semaphore_create(int initialValue);

/* Wait until semaphore value is greater than zero. Then decrease the semaphore value. */
PAL_API void
Semaphore_wait(Semaphore self);

PAL_API void
Semaphore_post(Semaphore self);

PAL_API void
Semaphore_destroy(Semaphore self);

/*! @} */

/*! @} */

#ifdef __cplusplus
}
#endif


#endif /* THREAD_HAL_H_ */
