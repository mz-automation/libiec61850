#ifndef PYIEC61850_EVENTHANDLER_HPP
#define PYIEC61850_EVENTHANDLER_HPP


#include "iec61850_client.h"
#include <string>
#include <Python.h>


class PyThreadStateLock
{
public:
    PyThreadStateLock(void)
    {
        state = PyGILState_Ensure( );
    }

    ~PyThreadStateLock(void)
    {
        PyGILState_Release( state );
    }

private:
    PyGILState_STATE state;
};



class EventHandler {
    public:
        virtual ~EventHandler() {}
        virtual void setReceivedData(void *i_data_p) = 0;
        virtual void trigger() = 0;
};


class EventSubscriber {
    public:
        // TODO: use a map to store and find the instantiated EventSubscriber
        static EventSubscriber* m_last_created_event_subscriber;

        EventSubscriber(): _event_handler_p(nullptr)
        {
            m_last_created_event_subscriber = this;

            // add python thread support
            Py_Initialize();
            PyEval_InitThreads();
        }

        virtual ~EventSubscriber()
        {
            deleteEventHandler();
            m_last_created_event_subscriber = nullptr;
        }

        virtual void subscribe() = 0;

        void deleteEventHandler()
        {
            if (_event_handler_p) {
                delete _event_handler_p;
            }
            _event_handler_p = nullptr;
        }

        void setEventHandler(EventHandler *i_event_handler_p)
        {
            deleteEventHandler();
            _event_handler_p = i_event_handler_p;
        }

        EventHandler *getEventHandler()
        {
            return _event_handler_p;
        }

    private:
        EventHandler *_event_handler_p;
};

#endif
