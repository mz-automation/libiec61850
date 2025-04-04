#include <stdio.h>
#include <stdlib.h>

#include "iec61850_server.h"
#include "hal_thread.h"
#include "lib_memory.h"

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
    
    uint8_t *mmsBuffer = (uint8_t *)GLOBAL_MALLOC(dataSize);
    if (mmsBuffer == NULL) {
        return 0;
    }

    MmsValue_encodeMmsData(mmsValue, mmsBuffer, 0, true);
    
    GLOBAL_FREEMEM(mmsBuffer);

    if (mmsValue != NULL) {
        MmsValue_delete(mmsValue);
    }

    return 0;
}