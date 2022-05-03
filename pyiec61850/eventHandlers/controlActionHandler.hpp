#ifndef PYIEC61850_CONTROLACTIONHANDLER_HPP
#define PYIEC61850_CONTROLACTIONHANDLER_HPP

#include "eventHandler.hpp"



/*
 *  Abstract class for processing the received 'Control' check events.
 */
class CheckHandlerForPython: public EventHandler {
    public:
        virtual ~CheckHandlerForPython() {}

        virtual void setReceivedData(void *i_data_p)
        {
            // copy the received data
            ControlAction *l_my_data_p = static_cast<ControlAction*>(i_data_p);
            _libiec61850_control_action = *l_my_data_p;
        }

        void setMmsValue(MmsValue *i_data_value)
        {
            _libiec61850_mms_value = i_data_value;
        }

        void setIedServer(IedServer *i_ied_server)
        {
            // copy the pointer and get to server
            IedServer *l_my_server_p = i_ied_server;
            _libiec61850_ied_server = *l_my_server_p;
        }
        
        void setTest(bool i_test)
        {
            _libiec61850_test = i_test;
        }
                
        void setInterlockCheck(bool i_interlock_check)
        {
            _libiec61850_interlock_check = i_interlock_check;
        }

        CheckHandlerResult getCheckHandlerResult()
        {
            return _libiec61850_check_handler_result;
        }

        CheckHandlerResult _libiec61850_check_handler_result; // Should be set during "trigger" callback function
        ControlAction  _libiec61850_control_action;
        MmsValue*  _libiec61850_mms_value;
        IedServer  _libiec61850_ied_server;
        bool  _libiec61850_test;
        bool  _libiec61850_interlock_check;
};



/*
 *  Abstract class for processing the received 'Control' wait for execution events.
 */
class WaitForExecutionHandlerForPython: public EventHandler {
    public:
        virtual ~WaitForExecutionHandlerForPython() {}

        virtual void setReceivedData(void *i_data_p)
        {
            // copy the received data
            ControlAction *l_my_data_p = static_cast<ControlAction*>(i_data_p);
            _libiec61850_control_action = *l_my_data_p;
        }

        void setMmsValue(MmsValue *i_data_value)
        {
            _libiec61850_mms_value = i_data_value;
        }

        void setIedServer(IedServer *i_ied_server)
        {
            // copy the pointer and get to server
            IedServer *l_my_server_p = i_ied_server;
            _libiec61850_ied_server = *l_my_server_p;
        }
        
        void setTest(bool i_test)
        {
            _libiec61850_test = i_test;
        }
                
        void setSynchroCheck(bool i_synchro_check)
        {
            _libiec61850_synchro_check = i_synchro_check;
        }

        ControlHandlerResult getControlHandlerResult()
        {
            return _libiec61850_control_handler_result;
        }

        ControlHandlerResult _libiec61850_control_handler_result; // Should be set during "trigger" callback function
        ControlAction  _libiec61850_control_action;
        MmsValue*  _libiec61850_mms_value;
        IedServer  _libiec61850_ied_server;
        bool  _libiec61850_test;
        bool  _libiec61850_synchro_check;

};



/*
 *  Abstract class for processing the received 'Control' events.
 */
class ControlHandlerForPython: public EventHandler {
    public:
        virtual ~ControlHandlerForPython() {}

        virtual void setReceivedData(void *i_data_p)
        {
            // copy the received data
            ControlAction *l_my_data_p = static_cast<ControlAction*>(i_data_p);
            _libiec61850_control_action = *l_my_data_p;
        }

        void setMmsValue(MmsValue *i_data_value)
        {
            _libiec61850_mms_value = i_data_value;
        }

        void setIedServer(IedServer *i_ied_server)
        {
            // copy the pointer and get to server
            IedServer *l_my_server_p = i_ied_server;
            _libiec61850_ied_server = *l_my_server_p;
        }
        
        void setTest(bool i_test)
        {
            _libiec61850_test = i_test;
        }

        ControlHandlerResult getControlHandlerResult()
        {
            return _libiec61850_control_handler_result;
        }

        ControlHandlerResult _libiec61850_control_handler_result; // Should be set during "trigger" callback function
        ControlAction  _libiec61850_control_action;
        MmsValue*  _libiec61850_mms_value;
        IedServer  _libiec61850_ied_server;
        bool  _libiec61850_test;
};


/*
 *  Class for the subscription to the 'Control' events
 */
class ControlSubscriberForPython: public EventSubscriber {
    public:
    
        ControlSubscriberForPython(): EventSubscriber()
        {
            m_ied_server = nullptr;
            m_control_object = nullptr;
            m_perform_check_handler_p = nullptr;
            m_wait_handler_p = nullptr;
            m_control_handler_p = nullptr;
        }

        virtual ~ControlSubscriberForPython() {
            if (m_perform_check_handler_p) {
                delete m_perform_check_handler_p;
            }
            m_perform_check_handler_p = nullptr;            
            if (m_wait_handler_p) {
                delete m_wait_handler_p;
            }
            m_wait_handler_p = nullptr;            
            if (m_control_handler_p) {
                delete m_control_handler_p;
            }
            m_control_handler_p = nullptr;
        }
        
        // Getters
        CheckHandlerForPython *getCheckHandler()
        {
            return m_perform_check_handler_p;
        }

        WaitForExecutionHandlerForPython *getWaitHandler()
        {
            return m_wait_handler_p;
        }

        ControlHandlerForPython *getControlHandler()
        {
            return m_control_handler_p;
        }

        // Subscriber: assign callbacks to IedServer
        virtual bool subscribe()
        {
            // preconditions
            if (nullptr == m_ied_server) {
                fprintf(stderr, "ControlSubscriber::subscribe() failed: 'ied server' is null\n");
                return false;
            }
            if (nullptr == m_control_object) {
                fprintf(stderr, "ControlSubscriber::subscribe() failed: 'control object' is null\n");
                return false;
            }

            // install the libiec61850 callbacks for the different control states, if handler are set:
            // the 'function pointer' is the 'static' method of this class
            if (m_perform_check_handler_p) {
                IedServer_setPerformCheckHandler(m_ied_server, 
                                                 m_control_object,
                                                 (ControlPerformCheckHandler) ControlSubscriberForPython::triggerPerformCheckHandler, 
                                                 &m_ied_server);
            }
            if (m_wait_handler_p){
                IedServer_setWaitForExecutionHandler(m_ied_server, 
                                                     m_control_object,
                                                     (ControlWaitForExecutionHandler) ControlSubscriberForPython::triggerWaitForExecutionHandler, 
                                                     &m_ied_server);
            }

            if (m_control_handler_p) {
                IedServer_setControlHandler(m_ied_server, 
                                            m_control_object,
                                            (ControlHandler) ControlSubscriberForPython::triggerControlHandler, 
                                            &m_ied_server);
            }

            char *l_object_ref = ModelNode_getObjectReference( (ModelNode*) m_control_object, NULL);
            std::string objs(l_object_ref);
            std::string dirs = std::string(ModelNode_getObjectReference( (ModelNode*) m_control_object, NULL));

            return (EventSubscriber::registerNewSubscriber(this, l_object_ref));
        }



        // Static method: it is the control perform check 'callback' for libiec61850 in C
        static CheckHandlerResult triggerPerformCheckHandler(ControlAction action, void* parameter, MmsValue* ctlVal, bool test, bool interlockCheck)
        {
            PyThreadStateLock PyThreadLock;

            // Preconditions
            if (nullptr == parameter) {
                fprintf(stderr, "ControlSubscriber::triggerPerformCheckHandler() failed: input object is null\n");
                return CONTROL_HARDWARE_FAULT; // Status may not 
            }

            DataObject* control_object = ControlAction_getControlObject(action);
            std::string l_object_ref = ModelNode_getObjectReference( (ModelNode*) control_object, NULL);

            // Search the appropriate 'EventSubscriber' object
            ControlSubscriberForPython *l_registered_subscriber = (ControlSubscriberForPython*) EventSubscriber::findSubscriber(l_object_ref);

            if (l_registered_subscriber) {
                CheckHandlerForPython *l_perform_check_handler_p = l_registered_subscriber->getCheckHandler();
                if (l_perform_check_handler_p) {
                    l_perform_check_handler_p->setReceivedData(&action);
                    l_perform_check_handler_p->setMmsValue(ctlVal);
                    l_perform_check_handler_p->setIedServer((IedServer*) parameter);
                    l_perform_check_handler_p->setTest(test);
                    l_perform_check_handler_p->setInterlockCheck(interlockCheck);
                    l_perform_check_handler_p->trigger();
                                            fprintf(stderr, "triggerPerformCheckHandler::triggerPerformCheckHandler() end\n");

                    return l_perform_check_handler_p->getCheckHandlerResult();
                }
                else {
                    fprintf(stderr, "ControlSubscriber::triggerPerformCheckHandler() failed: EventHandler is undefined\n");
                    return CONTROL_HARDWARE_FAULT;
                }
            }
            else {
                fprintf(stderr, "ControlSubscriber::triggerPerformCheckHandler() failed: subscriber is not registered\n");
                return CONTROL_HARDWARE_FAULT;
            }

        }

        // Static method: it is the control wait for execution 'callback' for libiec61850 in C
        static ControlHandlerResult triggerWaitForExecutionHandler(ControlAction controlAction, void* parameter, MmsValue* value, bool test, bool synchroCheck)
        {
            PyThreadStateLock PyThreadLock;

            // Preconditions
            if (nullptr == parameter) {
                fprintf(stderr, "ControlSubscriber::triggerWaitForExecutionHandler() failed: input object is null\n");
                return CONTROL_RESULT_FAILED;
            }

            DataObject* control_object = ControlAction_getControlObject(controlAction);
            std::string l_object_ref = ModelNode_getObjectReference( (ModelNode*) control_object, NULL);

            // Search the appropriate 'EventSubscriber' object
            ControlSubscriberForPython *l_registered_subscriber = (ControlSubscriberForPython*) EventSubscriber::findSubscriber(l_object_ref);

            if (l_registered_subscriber) {
                WaitForExecutionHandlerForPython *l_wait_handler_p = l_registered_subscriber->getWaitHandler();
                if (l_wait_handler_p) {
                    l_wait_handler_p->setReceivedData(&controlAction);
                    l_wait_handler_p->setMmsValue(value);
                    l_wait_handler_p->setIedServer((IedServer*) parameter);
                    l_wait_handler_p->setTest(test);
                    l_wait_handler_p->setSynchroCheck(synchroCheck);
                    l_wait_handler_p->trigger();
                                fprintf(stderr, "triggerWaitForExecutionHandler::triggerWaitForExecutionHandler() end\n");

                    return l_wait_handler_p->getControlHandlerResult();
                }
                else {
                    fprintf(stderr, "ControlSubscriber::triggerWaitForExecutionHandler() failed: EventHandler is undefined\n");
                    return CONTROL_RESULT_FAILED;
                }
            }
            else {
                fprintf(stderr, "ControlSubscriber::triggerWaitForExecutionHandler() failed: subscriber is not registered\n");
                return CONTROL_RESULT_FAILED;
            }

        }


        // Static method: it is the control 'callback' for libiec61850 in C
        static ControlHandlerResult triggerControlHandler(ControlAction controlAction, void* parameter, MmsValue* value, bool test)
        {
            PyThreadStateLock PyThreadLock;

            // Preconditions
            if (nullptr == parameter) {
                fprintf(stderr, "ControlSubscriber::triggerControlHandler() failed: input object is null\n");
                return CONTROL_RESULT_FAILED;
            }

            DataObject* control_object = ControlAction_getControlObject(controlAction);
            std::string l_object_ref = ModelNode_getObjectReference( (ModelNode*) control_object, NULL);

            // Search the appropriate 'EventSubscriber' object
            ControlSubscriberForPython *l_registered_subscriber = (ControlSubscriberForPython*) EventSubscriber::findSubscriber(l_object_ref);

            if (l_registered_subscriber) {
                ControlHandlerForPython *l_control_handler_p = l_registered_subscriber->getControlHandler();
                if (l_control_handler_p) {
                    l_control_handler_p->setReceivedData(&controlAction);
                    l_control_handler_p->setMmsValue(value);
                    l_control_handler_p->setIedServer((IedServer*) parameter);
                    l_control_handler_p->setTest(test);
                    l_control_handler_p->trigger();
                                fprintf(stderr, "triggerControlHandler::triggerControlHandler() end\n");

                    return l_control_handler_p->getControlHandlerResult();
                }
                else {
                    fprintf(stderr, "ControlSubscriber::triggerControlHandler() failed: EventHandler is undefined\n");
                    return CONTROL_RESULT_FAILED;
                }
            }
            else {
                fprintf(stderr, "ControlSubscriber::triggerControlHandler() failed: subscriber is not registered\n");
                return CONTROL_RESULT_FAILED;
            }

        }

        // Setters
        void setIedServer(const IedServer &i_ied_server)
        {
            m_ied_server = i_ied_server;
        }

        void setControlObject(DataObject *i_control_object)
        {
            m_control_object = i_control_object;
        }      

        void setCheckHandler(CheckHandlerForPython* i_check_handler_p)
        {
            m_perform_check_handler_p = i_check_handler_p;
        }      

        void setWaitHandler(WaitForExecutionHandlerForPython* i_wait_handler_p)
        {
            m_wait_handler_p = i_wait_handler_p;
        }      

        void setControlHandler(ControlHandlerForPython* i_control_handler_p)
        {
            m_control_handler_p = i_control_handler_p;
        }      

    private:
        // Parameters
        IedServer m_ied_server;
        DataObject* m_control_object;
        CheckHandlerForPython* m_perform_check_handler_p;
        WaitForExecutionHandlerForPython* m_wait_handler_p;
        ControlHandlerForPython* m_control_handler_p;

};

#endif
