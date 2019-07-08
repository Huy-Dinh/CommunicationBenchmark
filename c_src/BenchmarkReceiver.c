#include "BenchmarkReceiver.h"

void runStateMachine(BenchmarkReceiver_t* pReceiver, BenchmarkReceiverEvent_t evt, unsigned char * pBuffer, unsigned int size)
{
        switch(pReceiver->mState)
    {
        case RECEIVER_STATE_IDLE:
            if (evt == RECEIVER_EVT_CASE_BEGIN)
            {
                pReceiver->mState = RECEIVER_STATE_IN_CASE;
                pReceiver->mCurrentTestCase = pBuffer[1];
                if (pReceiver->pGetTickFunction != nullptr)
                    pReceiver->mTestCases[pReceiver->mCurrentTestCase].timeTaken = pReceiver->pGetTickFunction();
            }
            break;
        case RECEIVER_STATE_IN_CASE:
            /* If we receive a control word indicating a case has ended before
                having received a complete set of packets in the test case, the 
                case failed*/
            if (evt == RECEIVER_EVT_CASE_END)
            {
                pReceiver->mState = RECEIVER_STATE_IDLE;
                if (pReceiver->pGetTickFunction != nullptr)
                {
                    pReceiver->mTestCases[pReceiver->mCurrentTestCase].timeTaken = \
                        pReceiver->pGetTickFunction() - pReceiver->mTestCases[pReceiver->mCurrentTestCase].timeTaken;
                }
                printReceiveResult(&(pReceiver->mTestCases[pReceiver->mCurrentTestCase]));
            }
            else if (evt == RECEIVER_EVT_PACKET)
            {
                /* Successful test case */
                if (BENCHMARK_PACKET_CHECK_PASS_FINISH == 
                    checkReceivedPacket(&(pReceiver->mTestCases[pReceiver->mCurrentTestCase]), pBuffer, size))
                {
                    if (pReceiver->pGetTickFunction != nullptr)
                    {
                        pReceiver->mTestCases[pReceiver->mCurrentTestCase].timeTaken = \
                            pReceiver->pGetTickFunction() - pReceiver->mTestCases[pReceiver->mCurrentTestCase].timeTaken;
                    }
                    pReceiver->mState = RECEIVER_STATE_IDLE;
                    printReceiveResult(&(pReceiver->mTestCases[pReceiver->mCurrentTestCase]));
                }
            }
            break;
        default:
            break;
    }
}
void benchmarkReceive(BenchmarkReceiver_t* pReceiver, unsigned char * pBuffer, unsigned int size)
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
    runStateMachine(pReceiver, event, pBuffer, size);
}
