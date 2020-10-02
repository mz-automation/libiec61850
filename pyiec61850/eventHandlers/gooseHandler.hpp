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

        virtual void subscribe() {
            // install the libiec61850 callback:
            // the 'function pointer' is the 'static' method of this class
            GooseSubscriber_setListener(m_libiec61850_goose_subscriber,
                                        GooseSubscriberForPython::triggerGooseHandler,
                                        NULL);
        }

        // Static method: it is the 'callback' for libiec61850 in C
        static void triggerGooseHandler(GooseSubscriber subscriber, void *parameter)
        {
            PyThreadStateLock PyThreadLock;

            // TODO: search the appropriate 'EventSubscriber' object
            if (m_last_created_event_subscriber) {
                EventHandler *l_event_handler_p = m_last_created_event_subscriber->getEventHandler();
                if (l_event_handler_p) {
                    l_event_handler_p->setReceivedData(&subscriber);
                    l_event_handler_p->trigger();
                }
                else {
                    printf("The EventHandler is undefined\n");
                }
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
