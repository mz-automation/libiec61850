/*
 *  linked_list.h
 *
 *  Copyright 2013-2021 Michael Zillgith
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

#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include "libiec61850_common_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup common_api_group
 */
/**@{*/

/**
 * \defgroup LINKED_LIST LinkedList data type definition and handling functions
 */
/**@{*/


struct sLinkedList {
	void* data;
	struct sLinkedList* next;
};

/**
 * \brief Reference to a linked list or to a linked list element.
 */
typedef struct sLinkedList* LinkedList;

/**
 * \brief Create a new LinkedList object
 *
 * \return the newly created LinkedList instance
 */
LIB61850_API LinkedList
LinkedList_create(void);

/**
 * \brief Delete a LinkedList object
 *
 * This function destroy the LinkedList object. It will free all data structures used by the LinkedList
 * instance. It will call free for all elements of the linked list. This function should only be used if
 * simple objects (like dynamically allocated strings) are stored in the linked list.
 *
 * \param self the LinkedList instance
 */
LIB61850_API void
LinkedList_destroy(LinkedList self);


typedef void (*LinkedListValueDeleteFunction) (void*);

/**
 * \brief Delete a LinkedList object
 *
 * This function destroy the LinkedList object. It will free all data structures used by the LinkedList
 * instance. It will call a user provided function for each data element. This user provided function is
 * responsible to properly free the data element.
 *
 * \param self the LinkedList instance
 * \param valueDeleteFunction a function that is called for each data element of the LinkedList with the pointer
 *         to the linked list data element.
 */
LIB61850_API void
LinkedList_destroyDeep(LinkedList self, LinkedListValueDeleteFunction valueDeleteFunction);

/**
 * \brief Delete a LinkedList object without freeing the element data
 *
 * This function should be used statically allocated data objects are stored in the LinkedList instance.
 * Other use cases would be if the data elements in the list should not be deleted.
 *
 * \param self the LinkedList instance
 */
LIB61850_API void
LinkedList_destroyStatic(LinkedList self);

/**
 * \brief Add a new element to the list
 *
 * This function will add a new data element to the list. The new element will the last element in the
 * list.
 *
 * \param self the LinkedList instance
 * \param data data to append to the LinkedList instance
 */
LIB61850_API void
LinkedList_add(LinkedList self, void* data);

/**
 * \brief Check if the specified data is contained in the list
 *
 * \param self the LinkedList instance
 * \param data data to remove from the LinkedList instance
 *
 * \return true if data is part of the list, false otherwise
 */
LIB61850_API bool
LinkedList_contains(LinkedList self, void* data);

/**
 * \brief Removed the specified element from the list
 *
 * \param self the LinkedList instance
 * \param data data to remove from the LinkedList instance
 *
 *  \return true if data has been removed from the list, false otherwise
 */
LIB61850_API bool
LinkedList_remove(LinkedList self, void* data);

/**
 * \brief Get the list element specified by index (starting with 0).
 *
 * \param self the LinkedList instance
 * \param index index of the requested element.
 */
LIB61850_API LinkedList
LinkedList_get(LinkedList self, int index);

/**
 * \brief Get the next element in the list (iterator).
 *
 * \param self the LinkedList instance
 */
LIB61850_API LinkedList
LinkedList_getNext(LinkedList self);

/**
 * \brief Get the last element in the list.
 *
 * \param self the LinkedList instance
 */
LIB61850_API LinkedList
LinkedList_getLastElement(LinkedList self);

/**
 * \brief Insert a new element int the list
 *
 * \param listElement the LinkedList instance
 */
LIB61850_API LinkedList
LinkedList_insertAfter(LinkedList listElement, void* data);

/**
 * \brief Get the size of the list
 *
 * \param self the LinkedList instance
 *
 * \return number of data elements stored in the list
 */
LIB61850_API int
LinkedList_size(LinkedList self);

LIB61850_API void*
LinkedList_getData(LinkedList self);

LIB61850_API void
LinkedList_printStringList(LinkedList self);

/**@}*/

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* LINKED_LIST_H_ */
