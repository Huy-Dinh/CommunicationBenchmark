#ifndef BENCHMARKCONFIG_H_
#define BENCHMARKCONFIG_H_

#include <stdio.h> 

#ifndef NULL
    #define NULL 0
#endif

#ifndef nullptr
    #define nullptr NULL
#endif

/* Printing configuration */
#define BENCHMARK_ENABLE_PRINT      1
#define BENCHMARK_PRINT_FUNCTION    printf // Route to other print function if necessary

/* The datatype used for time */
typedef unsigned long BenchmarkTime_t;

/* Number of failed  */
#define BENCHMARK_MAX_SEND_FAILURE  30

#if BENCHMARK_ENABLE_PRINT
    #define benchmarkPrint(...) BENCHMARK_PRINT_FUNCTION(__VA_ARGS__)
#else
    #define benchmarkPrint(...)
#endif


/* The result of the sending attempt */
typedef enum 
{
    BENCHMARK_SEND_UNDECIDED,
    BENCHMARK_SEND_PASS,
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

/* Callback function pointer declarations */
typedef BenchmarkSendVerdict_t (*sendFuncPtr_t)(unsigned char *, unsigned int);
typedef void (*delayFuncPtr_t)(BenchmarkTime_t);
typedef BenchmarkTime_t (*getTickFuncPtr_t)();

#endif
