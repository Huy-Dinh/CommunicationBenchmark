#ifndef BENCHMARKSENDER_H
#define BENCHMARKSENDER_H

#include "BenchmarkConfig.h"
#include "BenchmarkTestCase.h"

typedef struct BenchmarkSender 
{
    BenchmarkTestCase_t* mTestCases;
    unsigned int mNumberOfTestCases;
    unsigned long mDelayBetweenTestCases;
    unsigned int mCurrentTestCase;
    sendFuncPtr_t pSendFunction;
    getTickFuncPtr_t pGetTickFunction;
} BenchmarkSender_t;


void runSend(BenchmarkSender_t* pSender);

#endif