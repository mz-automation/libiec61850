#include <stdio.h>
#include <stdlib.h>

#include "iec61850_server.h"
#include "iso_presentation.h"

int LLVMFuzzerTestOneInput(const char *data, size_t size) {
    IsoPresentation pres;

    ByteBuffer byteBuffer;
    byteBuffer.buffer = (uint8_t*) data;
    byteBuffer.maxSize = size;
    byteBuffer.size = size;

    int res = IsoPresentation_parseAcceptMessage(&pres, &byteBuffer);

    return 0;
}
