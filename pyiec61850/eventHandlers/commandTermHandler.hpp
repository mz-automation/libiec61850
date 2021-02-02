#ifndef PYIEC61850_COMMANDTERMHANDLER_HPP
#define PYIEC61850_COMMANDTERMHANDLER_HPP

#include "eventHandler.hpp"

/*
 *  Abstract class for processing the received 'Command Termination' events.
 */
class CommandTermHandler: public EventHandler {
    public:
        virtual ~CommandTermHandler() {}

        virtual void setReceivedData(void *i_data_p)
        {
            // copy the received data
            ControlObjectClient *l_my_data_p = static_cast<ControlObjectClient*>(i_data_p);
            _libiec61850_control_object_client = *l_my_data_p;
        }

        ControlObjectClient  _libiec61850_control_object_client;
};


/*
 *  Class for the subscription to the 'Command Termination' events
 */
class CommandTermSubscriber: public EventSubscriber {
    public:

        virtual void subscribe() {
            // install the libiec61850 callback:
            // the 'function pointer' is the 'static' method of this class
            ControlObjectClient_setCommandTerminationHandler(
                                m_libiec61850_control_object_client,
                                CommandTermSubscriber::triggerCommandTermHandler,
                                NULL);
        }

        // Static method: it is the 'callback' for libiec61850 in C
        static void triggerCommandTermHandler(void *parameter, ControlObjectClient connection)
        {
            PyThreadStateLock PyThreadLock;

            // TODO: search the appropriate 'EventSubscriber' object
            if (m_last_created_event_subscriber) {
                EventHandler *l_event_handler_p = m_last_created_event_subscriber->getEventHandler();
                if (l_event_handler_p) {
                    l_event_handler_p->setReceivedData(&connection);
                    l_event_handler_p->trigger();
                }
                else {
                    printf("The EventHandler is undefined\n");
                }
            }
        }

        // Setters
        void setLibiec61850ControlObjectClient(const ControlObjectClient &i_libiec61850_control_object_client)
        {
            m_libiec61850_control_object_client = i_libiec61850_control_object_client;
        }

    protected:
        // Parameters
        ControlObjectClient m_libiec61850_control_object_client;
};

#endif
