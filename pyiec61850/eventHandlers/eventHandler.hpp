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
        EventHandler() {}
        virtual ~EventHandler() {}
        virtual void setReceivedData(void *i_data_p) = 0;
        virtual void trigger() = 0;
};


class EventSubscriber {
    public:

        EventSubscriber(): _event_handler_p(nullptr)
        {
            // add python thread support
            Py_Initialize();
            PyEval_InitThreads();
        }

        virtual ~EventSubscriber()
        {
            EventSubscriber::unregisterSubscriber(m_subscriber_id);
            deleteEventHandler();
        }

        virtual bool subscribe() = 0;

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

        void setSubscriberId(const std::string &i_id)
        {
            m_subscriber_id = i_id;
        }

    protected:
        static std::map<std::string, EventSubscriber*> m_subscriber_map;

        static bool registerNewSubscriber(EventSubscriber *i_new_subscriber, const std::string &i_id)
        {
            // Preconditions
            if (i_id.empty()) {
                fprintf(stderr, "EventSubscriber::subscribe() failed: the subscriber id is empty\n");
                return false;
            }
            if (m_subscriber_map.end() != m_subscriber_map.find(i_id)) {
                fprintf(stderr, "EventSubscriber::subscribe() failed: the subscriber is already registered\n");
                return false;
            }

            m_subscriber_map[i_id] = i_new_subscriber;
            i_new_subscriber->setSubscriberId(i_id);

            return true;
        }

        static EventSubscriber* findSubscriber(const std::string &i_id)
        {
            EventSubscriber *o_found_event_subscriber_p = nullptr;
            std::map<std::string, EventSubscriber*>::iterator l_it = m_subscriber_map.find(i_id);

            if (m_subscriber_map.end() != l_it) {
                o_found_event_subscriber_p = l_it->second;
            }

            return o_found_event_subscriber_p;
        }

        static void unregisterSubscriber(const std::string &i_subscriber_id)
        {
            std::map<std::string, EventSubscriber*>::iterator l_it = m_subscriber_map.find(i_subscriber_id);

            if (m_subscriber_map.end() != l_it) {
                m_subscriber_map.erase(l_it);
            }
            else {
                fprintf(stderr, "EventSubscriber::unregisterSubscriber() failed: '%s' is not registered\n", i_subscriber_id.c_str());
            }
        }

    private:
        EventHandler *_event_handler_p;
        std::string m_subscriber_id;
};

#endif
