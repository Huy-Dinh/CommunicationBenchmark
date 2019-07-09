#include "BenchmarkSender.h"

void runSend(BenchmarkSender_t* pSender)
{
    /* Check preconditions, no need to check send and getTick because they are
    already checked inside the test case */
    if (pSender->mTestCases == nullptr) return;
    if (pSender->pGetTickFunction == nullptr) return;

    if (pSender->mCurrentTestCase < pSender->mNumberOfTestCases)
    {
        if (pSender->pGetTickFunction() - pSender->mLastTestCaseTime > pSender->mDelayBetweenTestCases)
        {
            if (pSender->mTestCases[pSender->mCurrentTestCase].mSendResult.noOfPacketsSent == 0)
            {
                unsigned char controlWord[2] = {(unsigned char) BENCHMARK_CTRL_START_CASE, pSender->mCurrentTestCase};
                if ((*pSender->pSendFunction)(controlWord, 2) == BENCHMARK_SEND_FAIL)
                    return;
            }
            BenchmarkSendResult_t sendResult = runSendTestCase(&(pSender->mTestCases[pSender->mCurrentTestCase]), 
                                                            pSender->pSendFunction, pSender->pGetTickFunction);
            if (sendResult.verdict != BENCHMARK_SEND_UNDECIDED)
            {
                unsigned char controlWord[2] = {(unsigned char) BENCHMARK_CTRL_END_CASE, pSender->mCurrentTestCase};
                if ((*pSender->pSendFunction)(controlWord, 2) == BENCHMARK_SEND_FAIL)
                    return;
                    
                printSendResult(&(pSender->mTestCases[pSender->mCurrentTestCase]));
                ++(pSender->mCurrentTestCase);
                pSender->mLastTestCaseTime = pSender->pGetTickFunction();
            }
        }
    }
}

void runThroughputTest(BenchmarkSender_t* pSender)
{
    if (pSender->mTestCases == nullptr) return;

    if (pSender->mCurrentTestCase < pSender->mNumberOfTestCases)
    {
        if (pSender->pGetTickFunction() - pSender->mLastTestCaseTime > pSender->mDelayBetweenTestCases)
        {
            if (pSender->mTestCases[pSender->mCurrentTestCase].mSendResult.noOfPacketsSent == 0)
            {
                unsigned char controlWord[2] = {(unsigned char) BENCHMARK_CTRL_START_CASE, pSender->mCurrentTestCase};
                if ((*pSender->pSendFunction)(controlWord, 2) == BENCHMARK_SEND_FAIL)
                    return;
                pSender->mTestCases[pSender->mCurrentTestCase].timeTaken = pSender->pGetTickFunction();
            }
            BenchmarkSendResult_t sendResult = runThroughputTestCase(&(pSender->mTestCases[pSender->mCurrentTestCase]), pSender->pSendFunction);
            if (sendResult.verdict != BENCHMARK_SEND_UNDECIDED)
            {
                unsigned char controlWord[2] = {(unsigned char) BENCHMARK_CTRL_END_CASE, pSender->mCurrentTestCase};
                if((*pSender->pSendFunction)(controlWord, 2) == BENCHMARK_SEND_FAIL)
                    return;

                pSender->mTestCases[pSender->mCurrentTestCase].timeTaken = pSender->pGetTickFunction() - pSender->mTestCases[pSender->mCurrentTestCase].timeTaken;
                printSendResult(&(pSender->mTestCases[pSender->mCurrentTestCase]));
                ++(pSender->mCurrentTestCase);
                pSender->mLastTestCaseTime = pSender->pGetTickFunction();
            }
        }
    }
}
