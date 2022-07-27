#include <stdio.h>
#include <stdlib.h>

#include "goose_receiver.h"
#include "goose_subscriber.h"
#include "hal_thread.h"

static void
test_GooseSubscriberWithFixedLengthEncoding_gooseListener(GooseSubscriber subscriber, void* parameter)
{
}

int LLVMFuzzerTestOneInput(const char* data, size_t size)
{
    GooseReceiver receiver = GooseReceiver_create();

    GooseSubscriber subscriber = GooseSubscriber_create("KEMASIMLD1/LLN0$GO$PingPRVDO", NULL);

    uint8_t dstMac[6] = {0x01,0x0c,0xcd,0x01,0x00,0xee};
    GooseSubscriber_setDstMac(subscriber, dstMac);
    GooseSubscriber_setAppId(subscriber, 4656);

    GooseSubscriber_setListener(subscriber, test_GooseSubscriberWithFixedLengthEncoding_gooseListener, NULL);

    GooseReceiver_addSubscriber(receiver, subscriber);

    GooseReceiver_startThreadless(receiver);

    GooseReceiver_handleMessage(receiver, data, size);

    GooseReceiver_stop(receiver);

    GooseReceiver_destroy(receiver);
}