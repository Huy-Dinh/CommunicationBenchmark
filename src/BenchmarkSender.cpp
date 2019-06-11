#include "BenchmarkSender.h"

BenchmarkSender::BenchmarkSender(BenchmarkTestCase* testCases, unsigned int numberOfTestCase, sendFuncPtr_t sendFunction,
                                    delayFuncPtr_t delayFunction, getTickFuncPtr_t getTickFunction)
{
    mTestCases = testCases;
    mNumberOfTestCases = numberOfTestCase;
    pSendFunction = sendFunction;
    pDelayFunction = delayFunction;
    pGetTickFunction = getTickFunction;
}

void BenchmarkSender::runSend()
{
    /* Check preconditions, no need to check send and getTick because they are
        already checked inside the test case */
    if (mTestCases == nullptr) return;
    if (pDelayFunction == nullptr) return;

    /* Set the callbacks onto the test cases
        Because the callback attributes are static, this is done only once*/
    BenchmarkTestCase::setSendFunction(pSendFunction);
    BenchmarkTestCase::setDelayFunction(pDelayFunction);
    BenchmarkTestCase::setGetTickFunction(pGetTickFunction);
    
    /* Loop through the array and run every test case entry */
    for (unsigned int i = 0; i < mNumberOfTestCases; ++i)
    {
        mTestCases[i].runSend();
        mTestCases[i].printSendResult();
        (*pDelayFunction)(mDelayBetweenTestCases);
    }
}
