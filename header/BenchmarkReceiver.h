#ifndef BENCHMARKRECEIVER_H_
#define BENCHMARKRECEIVER_H_

#include "BenchmarkTestCase.h"

class BenchmarkReceiver
{
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

private:
    BenchmarkTestCase* mTestCases;
    unsigned char mNumberOfTestCases;
    unsigned char mCurrentTestCase;
    BenchmarkReceiverState_t mState;
    getTickFuncPtr_t pGetTickFunction;
    void runStateMachine(BenchmarkReceiverEvent_t evt, unsigned char * pBuffer, unsigned int size);
public:
    BenchmarkReceiver(BenchmarkTestCase * testCases, unsigned int numberOfTestCases, getTickFuncPtr_t getTickFunction);
    void receive(unsigned char * pBuffer, unsigned int size);
};

#endif
