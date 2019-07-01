#ifndef BENCHMARKRECEIVER_H_
#define BENCHMARKRECEIVER_H_

#include "BenchmarkTestCase.h"

typedef enum
{
    RECEIVER_STATE_IDLE = 0,
    RECEIVER_STATE_IN_CASE,
    RECEIVER_INVALID_STATE
} BenchmarkReceiverState_t;

typedef enum
{
    RECEIVER_EVT_CASE_BEGIN,
    RECEIVER_EVT_PACKET,
    RECEIVER_EVT_CASE_END,
    RECEIVER_EVT_INVALID
} BenchmarkReceiverEvent_t;

typedef struct BenchmarkReceiver
{
    BenchmarkTestCase_t* mTestCases;
    unsigned int mNumberOfTestCases;
    unsigned int mCurrentTestCase;
    BenchmarkReceiverState_t mState;
} BenchmarkReceiver_t;

void benchmarkReceive(BenchmarkReceiver_t* pReceiver, unsigned char * pBuffer, unsigned int size);

#endif
