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
    BenchmarkSendResult_t mSendResult;
    BenchmarkReceiveResult_t mReceiveResult;
} BenchmarkTestCase_t;

BenchmarkSendResult_t runSendPacket(BenchmarkTestCase_t* pTestCase, sendFuncPtr_t pSendFunction, delayFuncPtr_t pDelayFunction, getTickFuncPtr_t pGetTickFunction);
BenchmarkPacketCheckResult_t checkReceivedPacket(BenchmarkTestCase_t* pTestCase, unsigned char * pBuffer, unsigned int length);
void printSendResult(BenchmarkTestCase_t* pTestCase);
void printReceiveResult(BenchmarkTestCase_t* pTestCase);

#endif
