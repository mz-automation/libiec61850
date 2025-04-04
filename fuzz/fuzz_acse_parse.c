#include "acse.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    AcseConnection acseConnection;
    AcseConnection_init(&acseConnection, NULL, NULL, NULL);

    ByteBuffer* acseBuffer = ByteBuffer_create(NULL, size);
    ByteBuffer_append(acseBuffer, data, size);
    AcseConnection_parseMessage(&acseConnection, acseBuffer);
    
    ByteBuffer_destroy(acseBuffer);
    return 0;
}
