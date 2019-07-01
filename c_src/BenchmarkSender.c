#include "BenchmarkSender.h"

void runSend(BenchmarkSender_t* pSender)
{
    /* Check preconditions, no need to check send and getTick because they are
    already checked inside the test case */
    if (pSender->mTestCases == nullptr) return;
    if (pSender->pDelayFunction == nullptr) return;

    /* Loop through the array and run every test case entry */
    for (unsigned int i = 0; i < pSender->mNumberOfTestCases; ++i)
    {
        /* Send starting control word */
        unsigned char controlWord[2] = {(unsigned char) BENCHMARK_CTRL_START_CASE, (unsigned char) i};
        (*pSender->pSendFunction)(controlWord, 2);

        /* Send benchmark payloads */
        runSendPacket(&(pSender->mTestCases[i]), pSender->pSendFunction, pSender->pDelayFunction, pSender->pGetTickFunction);
        printSendResult(&(pSender->mTestCases[i]));

        /* Send ending control word */
        controlWord[0] = (unsigned char) BENCHMARK_CTRL_END_CASE;
        (*pSender->pSendFunction)(controlWord, 2);

        (*pSender->pDelayFunction)(pSender->mDelayBetweenTestCases);
    }
}
