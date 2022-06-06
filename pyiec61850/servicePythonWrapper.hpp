#ifndef PYIEC61850_SERVICEPYTHONWRAPPER_HPP
#define PYIEC61850_SERVICEPYTHONWRAPPER_HPP

#include "iec61850_client.h"
#include <Python.h>


class PyThreadStateSave
{
public:
    PyThreadStateSave(void)
    {
        if (PyGILState_Check())
            state = PyEval_SaveThread();
        else
            state = nullptr;
    }

    ~PyThreadStateSave(void)
    {
        if (state)
            PyEval_RestoreThread(state);
    }

private:
    PyThreadState* state;
};

/*
Wrapping of synchronous functions to prevent deadlocks
*/
static MmsValue* 
pyWrap_IedConnection_readObject(IedConnection con, IedClientError* error, const char* objRef, FunctionalConstraint fc)
{
    PyThreadStateSave gilStateSave;
    return IedConnection_readObject(con, error, objRef, fc);
}

static void
pyWrap_IedConnection_writeObject(IedConnection con, IedClientError* error, const char* objectReference,
        FunctionalConstraint fc, MmsValue* value)
{
    PyThreadStateSave gilStateSave;
    IedConnection_writeObject(con, error, objectReference, fc, value);
}

static void 
pyWrap_IedConnection_setRCBValues(IedConnection con, IedClientError* error, ClientReportControlBlock rcb, uint32_t parametersMask, 
        bool singleRequest)
{
    PyThreadStateSave gilStateSave;
    IedConnection_setRCBValues(con, error, rcb, parametersMask, singleRequest);
}

static ClientReportControlBlock 
pyWrap_IedConnection_getRCBValues(IedConnection con, IedClientError* error, const char* rcbReference,
        ClientReportControlBlock updateRcb)
{
    PyThreadStateSave gilStateSave;
    return IedConnection_getRCBValues(con, error, rcbReference, updateRcb);
}

#endif
