#ifndef BENCHMARKSENDER_H
#define BENCHMARKSENDER_H

#include "BenchmarkConfig.h"
#include "BenchmarkTestCase.h"

class BenchmarkSender
{
private:
    BenchmarkTestCase* mTestCases;
    unsigned int mNumberOfTestCases;
    unsigned long mDelayBetweenTestCases;
    sendFuncPtr_t pSendFunction;
    delayFuncPtr_t pDelayFunction;
    getTickFuncPtr_t pGetTickFunction;
public:
    BenchmarkSender(BenchmarkTestCase* testCases, unsigned int numberOfTestCase, sendFuncPtr_t sendFunction,
                    delayFuncPtr_t delayFunction, getTickFuncPtr_t getTickFunction);
    void runSend();
};

#endif
