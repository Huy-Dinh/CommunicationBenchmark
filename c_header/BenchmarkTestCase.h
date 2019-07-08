#ifndef BENCHMARKTESTCASE_H_
#define BENCHMARKTESTCASE_H_

#include "BenchmarkConfig.h"

typedef struct BenchmarkTestCase
{
    char* mTestCaseName;
    unsigned int mPacketSize;
    unsigned int mNumberOfPacket;
    unsigned int mPacketDelay;
    unsigned char * pDataBuffer;
    BenchmarkTime_t lastSentTimestamp;
    BenchmarkTime_t timeTaken;
    BenchmarkSendResult_t mSendResult;
    BenchmarkReceiveResult_t mReceiveResult;
} BenchmarkTestCase_t;

BenchmarkSendResult_t runSendTestCase(BenchmarkTestCase_t* pTestCase, sendFuncPtr_t pSendFunction, getTickFuncPtr_t pGetTickFunction);
BenchmarkSendResult_t runThroughputTestCase(BenchmarkTestCase_t* pTestCase, sendFuncPtr_t pSendFunction);
BenchmarkPacketCheckResult_t checkReceivedPacket(BenchmarkTestCase_t* pTestCase, unsigned char * pBuffer, unsigned int length);
void printSendResult(BenchmarkTestCase_t* pTestCase);
void printReceiveResult(BenchmarkTestCase_t* pTestCase);

#endif
