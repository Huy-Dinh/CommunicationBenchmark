#ifndef BENCHMARKSENDER_H
#define BENCHMARKSENDER_H

#include "BenchmarkConfig.h"
#include "BenchmarkTestCase.h"

class BenchmarkSender
{
private:
    BenchmarkTestCase* mTestCases;
    unsigned char mNumberOfTestCases;
    unsigned long mDelayBetweenTestCases;
    unsigned char mCurrentTestCase;
    BenchmarkTime_t mLastTestCaseTime;
    sendFuncPtr_t pSendFunction;
    getTickFuncPtr_t pGetTickFunction;
    delayFuncPtr_t pDelayFunction;
public:
    BenchmarkSender(BenchmarkTestCase* testCases, unsigned int numberOfTestCase, sendFuncPtr_t sendFunction,
                    delayFuncPtr_t delayFunction, getTickFuncPtr_t getTickFunction, unsigned long delayBetweenTestCases);

    void runSend();
    void runSendBlocking();
    void runThroughputTest();
};

#endif
