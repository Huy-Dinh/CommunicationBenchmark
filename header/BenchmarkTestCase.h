#ifndef BENCHMARKTESTCASE_H
#define BENCHMARKTESTCASE_H

#include <iostream>
#include "BenchmarkConfig.h"

/* The result of the sending attempt */
typedef enum
{
    BENCHMARK_SEND_PASS = 0,
    BENCHMARK_SEND_PASS_WITH_MISSED_DEADLINES,
    BENCHMARK_SEND_FAIL
} BenchmarkSendVerdict_t;

typedef struct 
{
    BenchmarkSendVerdict_t verdict;
    unsigned int noOfPacketsSent;
    unsigned int noOfMissedDeadlines;
} BenchmarkSendResult_t;

/* The result of the packet check after receiving */
typedef enum
{
    BENCHMARK_PACKET_CHECK_PASS = 0,
    BENCHMARK_PACKET_CHECK_FAIL,
    BENCHMARK_PACKET_CHECK_PASS_FINISH
} BenchmarkPacketCheckResult_t;

/* The result of the receive test case */
typedef enum
{
    BENCHMARK_RECEIVE_PASS = 0,
    BENCHMARK_RECEIVE_PASS_WITH_WRONG_PACKETS,
    BENCHMARK_RECEIVE_FAIL
} BenchmarkReceiveVerdict_t;

typedef struct
{
    BenchmarkReceiveVerdict_t verdict;
    unsigned int noOfReceivedPackets;
    unsigned int noOfWrongPackets;
} BenchmarkReceiveResult_t;

/* Control message starting bytes */
typedef enum
{
    BENCHMARK_CTRL_START_CASE = 123,
    BENCHMARK_CTRL_END_CASE
} BenchmarkControlByte_t;

typedef BenchmarkSendVerdict_t (*sendFuncPtr_t)(unsigned char *, unsigned int);
typedef void (*delayFuncPtr_t)(unsigned long);
typedef unsigned long (*getTickFuncPtr_t)();

class BenchmarkTestCase
{
private:
    std::string mTestCaseName;
    unsigned int mPacketSize;
    unsigned int mNumberOfPacket;
    unsigned int mPacketDelay;
    unsigned char * pDataBuffer;

    static sendFuncPtr_t pSendFunction;
    static delayFuncPtr_t pDelayFunction;
    static getTickFuncPtr_t pGetTickFunction;

    BenchmarkSendResult_t mSendResult;
    BenchmarkReceiveResult_t mReceiveResult;
public:
    BenchmarkTestCase(std::string testCaseName, unsigned int packetSize, 
                        unsigned int numberOfPacket, unsigned int packetDelay, 
                        unsigned char * dataPointer);
    BenchmarkSendResult_t runSend();
    BenchmarkPacketCheckResult_t checkReceivedPacket(unsigned char * pBuffer, unsigned int length);
    static void setSendFunction(sendFuncPtr_t sendFunction);
    static void setDelayFunction(delayFuncPtr_t delayFunction);
    static void setGetTickFunction(getTickFuncPtr_t getTickFunction);
    void printSendResult();
    void printReceiveResult();
    virtual ~BenchmarkTestCase()
    {}
};

#endif
