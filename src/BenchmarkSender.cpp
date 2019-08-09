#include "BenchmarkSender.h"

BenchmarkSender::BenchmarkSender(BenchmarkTestCase* testCases, unsigned int numberOfTestCase, sendFuncPtr_t sendFunction,
                    delayFuncPtr_t delayFunction, getTickFuncPtr_t getTickFunction, unsigned long delayBetweenTestCases)

{
    mTestCases = testCases;
    mNumberOfTestCases = numberOfTestCase;
    mCurrentTestCase = 0;
    pSendFunction = sendFunction;
    pGetTickFunction = getTickFunction;
    mDelayBetweenTestCases = delayBetweenTestCases;
    mLastTestCaseTime = 0;
}

void BenchmarkSender::runSend()
{
    /* Check preconditions, no need to check send and getTick because they are
    already checked inside the test case */
    if (mTestCases == nullptr) return;
    if (pGetTickFunction == nullptr || pSendFunction == nullptr) return;

    BenchmarkTestCase::setSendFunction(pSendFunction);
    BenchmarkTestCase::setGetTickFunction(pGetTickFunction);

    if (mCurrentTestCase < mNumberOfTestCases)
    {
        if (pGetTickFunction() - mLastTestCaseTime > mDelayBetweenTestCases)
        {
            if (mTestCases[mCurrentTestCase].mSendResult.noOfPacketsSent == 0)
            {
                unsigned char controlWord[2] = {(unsigned char) BENCHMARK_CTRL_START_CASE, mCurrentTestCase};
                if ((*pSendFunction)(controlWord, 2) != BENCHMARK_SEND_PASS)
                    return;
            }
            BenchmarkSendResult_t sendResult = mTestCases[mCurrentTestCase].runSend();
            if (sendResult.verdict != BENCHMARK_SEND_UNDECIDED)
            {
                unsigned char controlWord[2] = {(unsigned char) BENCHMARK_CTRL_END_CASE, mCurrentTestCase};
                if ((*pSendFunction)(controlWord, 2) != BENCHMARK_SEND_PASS)
                    return;

                mTestCases[mCurrentTestCase].printSendResult();
                ++mCurrentTestCase;
                mLastTestCaseTime = pGetTickFunction();
            }
        }
    }
}

void BenchmarkSender::runSendBlocking()
{
    /* Check preconditions*/
    if (mTestCases == nullptr) return;
    if (pDelayFunction == nullptr || pSendFunction == nullptr) return;

    BenchmarkTestCase::setSendFunction(pSendFunction);
    BenchmarkTestCase::setDelayFunction(pDelayFunction);

    while (mCurrentTestCase < mNumberOfTestCases)
    {
        if (pGetTickFunction() - mLastTestCaseTime > mDelayBetweenTestCases)
        {
            if (mTestCases[mCurrentTestCase].mSendResult.noOfPacketsSent == 0)
            {
                unsigned char controlWord[2] = {(unsigned char) BENCHMARK_CTRL_START_CASE, mCurrentTestCase};
                if ((*pSendFunction)(controlWord, 2) != BENCHMARK_SEND_PASS)
                    return;
            }
            BenchmarkSendResult_t sendResult = mTestCases[mCurrentTestCase].runSendBlocking();
            if (sendResult.verdict != BENCHMARK_SEND_UNDECIDED)
            {
                unsigned char controlWord[2] = {(unsigned char) BENCHMARK_CTRL_END_CASE, mCurrentTestCase};
                if ((*pSendFunction)(controlWord, 2) != BENCHMARK_SEND_PASS)
                    return;

                mTestCases[mCurrentTestCase].printSendResult();
                ++mCurrentTestCase;
                mLastTestCaseTime = pGetTickFunction();
            }
        }
    }
}

void BenchmarkSender::runThroughputTest()
{
    if (mTestCases == nullptr) return;
    if (pGetTickFunction == nullptr || pSendFunction == nullptr) return;

    BenchmarkTestCase::setSendFunction(pSendFunction);
    BenchmarkTestCase::setGetTickFunction(pGetTickFunction);

    if (mCurrentTestCase < mNumberOfTestCases)
    {
        if (pGetTickFunction() - mLastTestCaseTime > mDelayBetweenTestCases)
        {
            if (mTestCases[mCurrentTestCase].mSendResult.noOfPacketsSent == 0)
            {
                unsigned char controlWord[2] = {(unsigned char) BENCHMARK_CTRL_START_CASE, mCurrentTestCase};
                (*pSendFunction)(controlWord, 2);
                mTestCases[mCurrentTestCase].mTimeTaken = pGetTickFunction();
            }
            BenchmarkSendResult_t sendResult = mTestCases[mCurrentTestCase].runThroughputTest();
            if (sendResult.verdict != BENCHMARK_SEND_UNDECIDED)
            {
                unsigned char controlWord[2] = {(unsigned char) BENCHMARK_CTRL_END_CASE, mCurrentTestCase};
                (*pSendFunction)(controlWord, 2);

                mTestCases[mCurrentTestCase].mTimeTaken = pGetTickFunction() - mTestCases[mCurrentTestCase].mTimeTaken;
                mTestCases[mCurrentTestCase].printSendResult();
                ++(mCurrentTestCase);
                mLastTestCaseTime = pGetTickFunction();
            }
        }
    }
}