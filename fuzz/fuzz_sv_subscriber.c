/*
 * libFuzzer + ASan harness for the IEC 61850-9-2 Sampled Values (SV) subscriber
 * decode path — the gap noted in GHSA-jh37-3w86-6rw8 / #598: GOOSE is harnessed
 * in fuzz/, SV was not, and SV is not on OSS-Fuzz, which is why parseASDU()
 * numeric-field OOB reads sat unfound.
 *
 * Entry point: SVReceiver_handleL2Message() — the public API (added 2026-07)
 * that feeds a raw Ethernet SV frame straight into the library, driving the
 * full receive chain: parseSVMessage -> parseSVPayload -> parseSequenceOfASDU
 * -> parseASDU -> listener -> SVSubscriber_ASDU_get*() accessors.
 *
 * The listener exercises EVERY numeric getter, exactly as a real SV application
 * would. Those accessors perform fixed-width reads (SmpCnt 2, ConfRev 4,
 * RefrTm 8, SmpSynch/SmpMod 1, SmpRate 2). Before a96bd67 parseASDU stored a
 * bare pointer for a too-short field and the accessor read past it; a96bd67
 * rejects the short field before the pointer store, so the accessor sees NULL.
 *
 * EXACT-SIZE BUFFER: the frame is copied into an exact-size malloc'd buffer so a
 * 1-byte over-read at frame end hits ASan's redzone instead of stack padding.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sv_subscriber.h"
#include "hal_thread.h"

/* A real SV application reads the ASDU fields inside its listener callback.
 * Calling every accessor here is what turns a stored-but-too-short pointer into
 * an observable out-of-bounds read. */
static void
svListener(SVSubscriber subscriber, void* parameter, SVSubscriber_ASDU asdu)
{
    (void)subscriber;
    (void)parameter;

    (void)SVSubscriber_ASDU_getSmpCnt(asdu);   /* reads 2 bytes */
    (void)SVSubscriber_ASDU_getConfRev(asdu);  /* reads 4 bytes */
    (void)SVSubscriber_ASDU_getSmpSynch(asdu); /* reads 1 byte  */
    (void)SVSubscriber_ASDU_getSmpMod(asdu);   /* reads 1 byte  */
    (void)SVSubscriber_ASDU_getSmpRate(asdu);  /* reads 2 bytes */

    if (SVSubscriber_ASDU_hasRefrTm(asdu)) {
        (void)SVSubscriber_ASDU_getRefrTmAsMs(asdu); /* reads 8 bytes */
        (void)SVSubscriber_ASDU_getRefrTmAsNs(asdu); /* reads 8 bytes */
    }

    (void)SVSubscriber_ASDU_getSvId(asdu);
    (void)SVSubscriber_ASDU_getDataSize(asdu);
}

int
LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* exact-size copy: any read at buf[size] traps on the ASan redzone */
    uint8_t* buf = (uint8_t*)malloc(size ? size : 1);
    if (!buf) {
        return 0;
    }
    if (size) {
        memcpy(buf, data, size);
    }

    SVReceiver receiver = SVReceiver_create();
    /* accept any destination MAC so the fuzzer reaches the decoder */
    SVReceiver_disableDestAddrCheck(receiver);

    uint8_t ethAddr[6] = { 0x01, 0x0c, 0xcd, 0x04, 0x00, 0x00 };
    SVSubscriber subscriber = SVSubscriber_create(ethAddr, 0x4000 /* APPID */);
    SVSubscriber_setListener(subscriber, svListener, NULL);
    SVReceiver_addSubscriber(receiver, subscriber);

    SVReceiver_handleL2Message(receiver, buf, (int)size);

    SVReceiver_destroy(receiver);
    free(buf);
    return 0;
}
