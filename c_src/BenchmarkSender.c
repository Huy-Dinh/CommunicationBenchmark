#include "BenchmarkSender.h"

void runSend(BenchmarkSender_t* pSender)
{
    /* Check preconditions, no need to check send and getTick because they are
    already checked inside the test case */
    if (pSender->mTestCases == nullptr) return;

    if (pSender->mCurrentTestCase < pSender->mNumberOfTestCases)
    {
        if (pSender->mTestCases[pSender->mCurrentTestCase].mSendResult.noOfPacketsSent == 0)
        {
            unsigned char controlWord[2] = {(unsigned char) BENCHMARK_CTRL_START_CASE, pSender->mCurrentTestCase};
            (*pSender->pSendFunction)(controlWord, 2);
        }
        BenchmarkSendResult_t sendResult = runSendPacket(&(pSender->mTestCases[pSender->mCurrentTestCase]), 
                                                        pSender->pSendFunction, pSender->pGetTickFunction);
        if (sendResult.verdict != BENCHMARK_SEND_UNDECIDED)
        {
            unsigned char controlWord[2] = {(unsigned char) BENCHMARK_CTRL_END_CASE, pSender->mCurrentTestCase};
            (*pSender->pSendFunction)(controlWord, 2);

            printSendResult(&(pSender->mTestCases[pSender->mCurrentTestCase]));
            ++(pSender->mCurrentTestCase);
        }
    }
}
