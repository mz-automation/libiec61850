#ifndef PYIEC61850_GOOSEHANDLER_HPP
#define PYIEC61850_GOOSEHANDLER_HPP

#include "eventHandler.hpp"


class GooseHandler: public EventHandler {
    public:
        virtual ~GooseHandler() {}

        virtual void setReceivedData(void *i_data_p)
        {
            // copy the received data
            GooseSubscriber *l_my_data_p = static_cast<GooseSubscriber*>(i_data_p);
            _libiec61850_goose_subscriber = *l_my_data_p;
        }

        GooseSubscriber  _libiec61850_goose_subscriber;
};



class GooseSubscriberForPython: public EventSubscriber {
    public:
        GooseSubscriberForPython(): EventSubscriber()
        {
            m_libiec61850_goose_subscriber = nullptr;
        }

        virtual ~GooseSubscriberForPython() {}

        virtual bool subscribe()
        {
            // preconditions
            if (nullptr == m_libiec61850_goose_subscriber) {
                fprintf(stderr, "GooseSubscriberForPython::subscribe() failed: 'GOOSE subscriber' is null\n");
                return false;
            }

            // install the libiec61850 callback:
            // the 'function pointer' is the 'static' method of this class
            GooseSubscriber_setListener(m_libiec61850_goose_subscriber,
                                        GooseSubscriberForPython::triggerGooseHandler,
                                        NULL);

            std::string l_go_cb_ref = GooseSubscriber_getGoCbRef(m_libiec61850_goose_subscriber);

            return (EventSubscriber::registerNewSubscriber(this, l_go_cb_ref));
        }

        // Static method: it is the 'callback' for libiec61850 in C
        static void triggerGooseHandler(GooseSubscriber subscriber, void *parameter)
        {
            PyThreadStateLock PyThreadLock;

            // Preconditions
            if (nullptr == subscriber) {
                fprintf(stderr, "GooseSubscriberForPython::triggerGooseHandler() failed: input object is null\n");
                return;
            }

            // Search the appropriate 'EventSubscriber' object
            std::string l_subscriber_id = GooseSubscriber_getGoCbRef(subscriber);
            EventSubscriber *l_registered_subscriber = EventSubscriber::findSubscriber(l_subscriber_id);

            if (l_registered_subscriber) {
                EventHandler *l_event_handler_p = l_registered_subscriber->getEventHandler();
                if (l_event_handler_p) {
                    l_event_handler_p->setReceivedData(&subscriber);
                    l_event_handler_p->trigger();
                }
                else {
                    fprintf(stderr, "GooseSubscriberForPython::triggerGooseHandler() failed: EventHandler is undefined\n");
                }
            }
            else {
                fprintf(stderr, "GooseSubscriberForPython::triggerGooseHandler() failed: subscriber is not registered\n");
            }
        }

        // Setters
        void setLibiec61850GooseSubscriber(const GooseSubscriber &i_libiec61850_goose_subscriber)
        {
            m_libiec61850_goose_subscriber = i_libiec61850_goose_subscriber;
        }

    protected:
        // Parameters
        GooseSubscriber m_libiec61850_goose_subscriber;
};

#endif
