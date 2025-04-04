#include <stdio.h>
#include <stdlib.h>

#include "iec61850_server.h"
#include "hal_thread.h"

#define kBufSize 4096

int LLVMFuzzerTestOneInput(const char *data, size_t size) {
    int out;
    MmsValue* mmsValue = NULL;
    mmsValue = MmsValue_decodeMmsData(data, 0, size, &out);
    if (mmsValue == NULL) {
        return 0;
    }

    int dataSize = MmsValue_encodeMmsData(mmsValue, NULL, 0, false);
    if (dataSize <= 0) {
        return 0;
    }
    
    char printBuffer[kBufSize];
    MmsValue_printToBuffer(mmsValue, printBuffer, kBufSize);

    if (mmsValue != NULL) {
        MmsValue_delete(mmsValue);
    }

    return 0;
}