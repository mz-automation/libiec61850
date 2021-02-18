#ifndef PYIEC61850_RCBHANDLER_HPP
#define PYIEC61850_RCBHANDLER_HPP

#include "eventHandler.hpp"


class RCBHandler: public EventHandler {
    public:
        virtual ~RCBHandler() {}

        virtual void setReceivedData(void *i_data_p)
        {
            // copy the received data
            ClientReport *l_my_data_p = static_cast<ClientReport*>(i_data_p);
            _client_report = *l_my_data_p;
        }

        ClientReport  _client_report;
};



class RCBSubscriber: public EventSubscriber {
    public:
        RCBSubscriber(): EventSubscriber()
        {
            m_ied_connection = nullptr;
        }

        virtual ~RCBSubscriber() {}

        virtual bool subscribe() {
            // preconditions
            if (nullptr == m_ied_connection) {
                fprintf(stderr, "RCBSubscriber::subscribe() failed: 'IedConnection' is null\n");
                return false;
            }

            // install the libiec61850 callback:
            // the 'function pointer' is the 'static' method of this class
            IedConnection_installReportHandler(m_ied_connection,
                                               m_rcb_reference.c_str(),
                                               m_rcb_rpt_id.c_str(),
                                               RCBSubscriber::triggerRCBHandler,
                                               NULL);

            return (EventSubscriber::registerNewSubscriber(this, m_rcb_reference));
        }

        // Static method: it is the 'callback' for libiec61850 in C
        static void triggerRCBHandler(void *parameter, ClientReport report)
        {
            PyThreadStateLock PyThreadLock;

            // Preconditions
            if (nullptr == report) {
                fprintf(stderr, "RCBSubscriber::triggerRCBHandler() failed: input object is null\n");
                return;
            }

            // Search the appropriate 'EventSubscriber' object
            std::string l_subscriber_id = ClientReport_getRcbReference(report);
            EventSubscriber *l_registered_subscriber = EventSubscriber::findSubscriber(l_subscriber_id);

            if (l_registered_subscriber) {
                EventHandler *l_event_handler_p = l_registered_subscriber->getEventHandler();
                if (l_event_handler_p) {
                    l_event_handler_p->setReceivedData(&report);
                    l_event_handler_p->trigger();
                }
                else {
                    fprintf(stderr, "RCBSubscriber::triggerRCBHandler() failed: EventHandler is undefined\n");
                }
            }
            else {
                fprintf(stderr, "RCBSubscriber::triggerRCBHandler() failed: subscriber is not registered\n");
            }
        }

        // Setters
        void setIedConnection(const IedConnection &i_ied_connection) {m_ied_connection = i_ied_connection;}
        void setRcbReference(const char *i_rcb_reference) {m_rcb_reference = i_rcb_reference;}
        void setRcbRptId(const char *i_rcb_rpt_id) {m_rcb_rpt_id = i_rcb_rpt_id;}

    protected:
        // Parameters
        IedConnection m_ied_connection;
        std::string   m_rcb_reference;
        std::string   m_rcb_rpt_id;
};

#endif
