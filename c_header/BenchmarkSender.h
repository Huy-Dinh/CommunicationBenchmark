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


// Run the testcases with the specified interval
// returns fail when a failure to send is detected (full buffer oder so)
void runSend(BenchmarkSender_t* pSender);

// Try to send the test packets as fast as possible to work out the throughput
void runThroughputTest(BenchmarkSender_t* pSender);

#endif
