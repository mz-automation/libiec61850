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
        CommandTermSubscriber(): EventSubscriber()
        {
            m_libiec61850_control_object_client = nullptr;
        }

        virtual ~CommandTermSubscriber() {}

        virtual bool subscribe()
        {
            // preconditions
            if (nullptr == m_libiec61850_control_object_client) {
                fprintf(stderr, "CommandTermSubscriber::subscribe() failed: 'control object client' is null\n");
                return false;
            }

            // install the libiec61850 callback:
            // the 'function pointer' is the 'static' method of this class
            ControlObjectClient_setCommandTerminationHandler(
                                m_libiec61850_control_object_client,
                                CommandTermSubscriber::triggerCommandTermHandler,
                                NULL);

            std::string l_object_ref = ControlObjectClient_getObjectReference(m_libiec61850_control_object_client);

            return (EventSubscriber::registerNewSubscriber(this, l_object_ref));
        }

        // Static method: it is the 'callback' for libiec61850 in C
        static void triggerCommandTermHandler(void *parameter, ControlObjectClient connection)
        {
            PyThreadStateLock PyThreadLock;

            // Preconditions
            if (nullptr == connection) {
                fprintf(stderr, "CommandTermSubscriber::triggerCommandTermHandler() failed: input object is null\n");
                return;
            }

            // Search the appropriate 'EventSubscriber' object
            std::string l_subscriber_id = ControlObjectClient_getObjectReference(connection);
            EventSubscriber *l_registered_subscriber = EventSubscriber::findSubscriber(l_subscriber_id);

            if (l_registered_subscriber) {
                EventHandler *l_event_handler_p = l_registered_subscriber->getEventHandler();
                if (l_event_handler_p) {
                    l_event_handler_p->setReceivedData(&connection);
                    l_event_handler_p->trigger();
                }
                else {
                    fprintf(stderr, "CommandTermSubscriber::triggerCommandTermHandler() failed: EventHandler is undefined\n");
                }
            }
            else {
                fprintf(stderr, "CommandTermSubscriber::triggerCommandTermHandler() failed: subscriber is not registered\n");
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
