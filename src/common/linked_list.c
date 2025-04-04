/*
 *  linked_list.c
 *
 *  Copyright 2013, 2014 Michael Zillgith
 *
 *	This file is part of libIEC61850.
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

#include "libiec61850_platform_includes.h"
#include "linked_list.h"

LinkedList
LinkedList_getLastElement(LinkedList self)
{
    while (self->next != NULL) {
        self = self->next;
    }

    return self;
}

LinkedList
LinkedList_create()
{
    LinkedList self;

    self = (LinkedList) GLOBAL_MALLOC(sizeof(struct sLinkedList));

    if (self)
    {
        self->data = NULL;
        self->next = NULL;
    }

    return self;
}

/**
 * Destroy list (free). Also frees element data with helper function.
 */
void
LinkedList_destroyDeep(LinkedList self, LinkedListValueDeleteFunction valueDeleteFunction)
{
    if (self)
    {
        LinkedList nextElement = self;
        LinkedList currentElement;

        do
        {
            currentElement = nextElement;
            nextElement = currentElement->next;

            if (currentElement->data != NULL)
                valueDeleteFunction(currentElement->data);

            GLOBAL_FREEMEM(currentElement);
        }
        while (nextElement != NULL);
    }
}

void
LinkedList_destroy(LinkedList self)
{
    LinkedList_destroyDeep(self, Memory_free);
}

/**
 * Destroy list (free) without freeing the element data
 */
void
LinkedList_destroyStatic(LinkedList self)
{
    LinkedList nextElement = self;
    LinkedList currentElement;

    do
    {
        currentElement = nextElement;
        nextElement = currentElement->next;
        GLOBAL_FREEMEM(currentElement);
    }
    while (nextElement != NULL);
}

int
LinkedList_size(LinkedList self)
{
    LinkedList nextElement = self;
    int size = 0;

    while (nextElement->next != NULL)
    {
        nextElement = nextElement->next;
        size++;
    }

    return size;
}

void
LinkedList_add(LinkedList self, void* data)
{
    LinkedList newElement = LinkedList_create();

    newElement->data = data;

    LinkedList listEnd = LinkedList_getLastElement(self);

    listEnd->next = newElement;
}

bool
LinkedList_contains(LinkedList self, void* data)
{
    LinkedList currentElement = self->next;

    while (currentElement != NULL)
    {
        if (currentElement->data == data)
            return true;

        currentElement = currentElement->next;
    }

    return false;
}

bool
LinkedList_remove(LinkedList self, void* data)
{
    LinkedList lastElement = self;

    LinkedList currentElement = self->next;

    while (currentElement != NULL)
    {
        if (currentElement->data == data)
        {
            lastElement->next = currentElement->next;
            GLOBAL_FREEMEM(currentElement);
            return true;
        }

        lastElement = currentElement;
        currentElement = currentElement->next;
    }

    return false;
}

LinkedList
LinkedList_insertAfter(LinkedList self, void* data)
{
    LinkedList newElement = LinkedList_create();

    if (newElement)
    {
        newElement->data = data;
        newElement->next = LinkedList_getNext(self);

        self->next = newElement;
    }

    return newElement;
}

LinkedList
LinkedList_getNext(LinkedList self)
{
    return self->next;
}

LinkedList
LinkedList_get(LinkedList self, int index)
{
    LinkedList element = LinkedList_getNext(self);

    int i = 0;

    while (i < index)
    {
        element = LinkedList_getNext(element);

        if (element == NULL)
            return NULL;

        i++;
    }

    return element;
}

void*
LinkedList_getData(LinkedList self)
{
    return self->data;
}

void
LinkedList_printStringList(LinkedList self)
{
    LinkedList element = self;

    while ((element = LinkedList_getNext(element)) != NULL)
    {
        char* str = (char*) (element->data);
        printf("%s\n", str);
    }
}
