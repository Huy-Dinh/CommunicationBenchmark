#ifndef BENCHMARKSENDER_H
#define BENCHMARKSENDER_H

#include "BenchmarkConfig.h"
#include "BenchmarkTestCase.h"

typedef struct BenchmarkSender 
{
    BenchmarkTestCase_t* mTestCases;
    unsigned char mNumberOfTestCases;
    unsigned long mDelayBetweenTestCases;
    unsigned char mCurrentTestCase;
    sendFuncPtr_t pSendFunction;
    getTickFuncPtr_t pGetTickFunction;
    unsigned long mLastTestCaseTime;
} BenchmarkSender_t;


void runSend(BenchmarkSender_t* pSender);

#endif
