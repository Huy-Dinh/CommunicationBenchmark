#include "BenchmarkReceiver.h"

BenchmarkReceiver::BenchmarkReceiver(BenchmarkTestCase * testCases, unsigned int numberOfTestCases)
{
    mTestCases = testCases;
    mNumberOfTestCases = numberOfTestCases;
    mState = RECEIVER_STATE_IDLE;
    mCurrentTestCase = 255;
}

void BenchmarkReceiver::receive(unsigned char * pBuffer, unsigned int size)
{
    BenchmarkReceiverEvent_t event = RECEIVER_EVT_INVALID;
    if (size == 2)
    {
        /* Check the control word */
        switch (pBuffer[0])
        {
            case (unsigned char) BENCHMARK_CTRL_START_CASE:
                event = RECEIVER_EVT_CASE_BEGIN;
                break;
            case (unsigned char) BENCHMARK_CTRL_END_CASE:
                event = RECEIVER_EVT_CASE_END;
                break;
            default:
                break;
        }
    }
    else
    {
        event = RECEIVER_EVT_PACKET;
    }
    runStateMachine(event, pBuffer, size);
}

void BenchmarkReceiver::runStateMachine(BenchmarkReceiverEvent_t evt, unsigned char * pBuffer, unsigned int size)
{
    switch(mState)
    {
        case RECEIVER_STATE_IDLE:
            if (evt == RECEIVER_EVT_CASE_BEGIN)
            {
                mState = RECEIVER_STATE_IN_CASE;
                mCurrentTestCase = pBuffer[1];
            }
            break;
        case RECEIVER_STATE_IN_CASE:
            /* If we receive a control word indicating a case has ended before
                having received a complete set of packets in the test case, the 
                case failed*/
            if (evt == RECEIVER_EVT_CASE_END)
            {
                mState = RECEIVER_STATE_IDLE;
                mTestCases[mCurrentTestCase].printReceiveResult();
            }
            else if (evt == RECEIVER_EVT_PACKET)
            {
                /* Successful test case */
                if (BENCHMARK_PACKET_CHECK_PASS_FINISH == 
                    mTestCases[mCurrentTestCase].checkReceivedPacket(pBuffer, size))
                {
                    mState = RECEIVER_STATE_IDLE;
                    mTestCases[mCurrentTestCase].printReceiveResult();
                }
            }
            break;
    }
}
