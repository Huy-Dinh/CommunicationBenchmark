#include "BenchmarkTestCase.h"
#include <string.h> // memcpy

BenchmarkSendResult_t runSendTestCase(BenchmarkTestCase_t* pTestCase, sendFuncPtr_t pSendFunction, getTickFuncPtr_t pGetTickFunction)
{
    /* returns fail if the callbacks are not registered
    or the sending buffer pointer is not assigned to a valid buffer 
    or the length of the packet is 0*/   
    if (pSendFunction == nullptr || pGetTickFunction == nullptr || pTestCase->pDataBuffer == nullptr
        || pTestCase->mNumberOfPacket == 0)
    {
        pTestCase->mSendResult.verdict = BENCHMARK_SEND_FAIL;
        return pTestCase->mSendResult;
    }

    if (pTestCase->mSendResult.verdict == BENCHMARK_SEND_UNDECIDED)
    {
        BenchmarkTime_t currentTick = (*pGetTickFunction)();
        if (currentTick - pTestCase->lastSentTimestamp >= pTestCase->mPacketDelay)
        {
            /* If an attempt to send fails, the sending test fails */
            if ((*pSendFunction)(pTestCase->pDataBuffer, pTestCase->mPacketSize) != BENCHMARK_SEND_PASS)
            {
                pTestCase->mSendResult.verdict = BENCHMARK_SEND_FAIL;
            }
            else
            {
                /* Update the last sent timestamp */
                pTestCase->lastSentTimestamp = currentTick;
                /* Increase the counter for successfully sent bytes */
                if (++(pTestCase->mSendResult.noOfPacketsSent) == pTestCase->mNumberOfPacket)
                {
                    /* If we reach here it's assumed that every packets have been put into 
                    the buffer successfully, only the number of missed deadlines need to be checked */
                    pTestCase->mSendResult.verdict = (pTestCase->mSendResult.noOfMissedDeadlines == 0) ? BENCHMARK_SEND_PASS : BENCHMARK_SEND_PASS_WITH_MISSED_DEADLINES;
                }
           }      
        }        
    }
    return pTestCase->mSendResult;
}

BenchmarkSendResult_t runThroughputTestCase(BenchmarkTestCase_t* pTestCase, sendFuncPtr_t pSendFunction)
{
    
    /* returns fail if the callbacks are not registered
    or the sending buffer pointer is not assigned to a valid buffer 
    or the length of the packet is 0*/   
    if (pSendFunction == nullptr ||  pTestCase->pDataBuffer == nullptr
        || pTestCase->mNumberOfPacket == 0)
    {
        pTestCase->mSendResult.verdict = BENCHMARK_SEND_FAIL;
        return pTestCase->mSendResult;
    }

    static unsigned int failureCount = 0;

    if (pTestCase->mSendResult.verdict == BENCHMARK_SEND_UNDECIDED)
    {
        if ((*pSendFunction)(pTestCase->pDataBuffer, pTestCase->mPacketSize) != BENCHMARK_SEND_PASS)
        {
            /* If the number of send failure surpasses what is specified */
            if (++failureCount >= BENCHMARK_MAX_SEND_FAILURE)
            {
                pTestCase->mSendResult.verdict = BENCHMARK_SEND_FAIL;
            }
        }
        else
        {
            failureCount = 0;
            /* Increase the counter for successfully sent bytes */
            if (++(pTestCase->mSendResult.noOfPacketsSent) >= pTestCase->mNumberOfPacket)
            {
                /* If we reach here it's assumed that every packets have been put into 
                the buffer successfully, only the number of missed deadlines need to be checked */
                pTestCase->mSendResult.verdict = BENCHMARK_SEND_PASS;
            }
        }
        
    }
    return pTestCase->mSendResult;
}

BenchmarkPacketCheckResult_t checkReceivedPacket(BenchmarkTestCase_t* pTestCase, unsigned char * pBuffer, unsigned int length)
{
         /* if the buffer pointer is not assigned to a valid buffer*/
    if (pTestCase->pDataBuffer == nullptr)
    {
        return BENCHMARK_PACKET_CHECK_FAIL;
    }
    /* if the received packet length is different from the one specified in the test case
        it's a fail */
    if (length !=pTestCase-> mPacketSize)
    {
        ++(pTestCase->mReceiveResult.noOfWrongPackets);
        return BENCHMARK_PACKET_CHECK_FAIL;
    }
    /* compare the received buffer */
    if (memcmp(pBuffer, pTestCase->pDataBuffer,pTestCase-> mPacketSize) != 0)
    {
        ++(pTestCase->mReceiveResult.noOfWrongPackets);
        return BENCHMARK_PACKET_CHECK_FAIL;
    }
    /* if all the packets have been received correctly, the test case passed */
    if (++(pTestCase->mReceiveResult.noOfReceivedPackets) == pTestCase->mNumberOfPacket)
    {
        pTestCase->mReceiveResult.verdict = (pTestCase->mReceiveResult.noOfWrongPackets == 0) ? BENCHMARK_RECEIVE_PASS : BENCHMARK_RECEIVE_PASS_WITH_WRONG_PACKETS;
        return BENCHMARK_PACKET_CHECK_PASS_FINISH;
    }
    return BENCHMARK_PACKET_CHECK_PASS;
}

void printReceiveResult(BenchmarkTestCase_t* pTestCase)
{
    benchmarkPrint("BENCHMARK test case \"%s\" RECEIVE result: ", pTestCase->mTestCaseName);
    switch (pTestCase->mReceiveResult.verdict)
    {
        case BENCHMARK_RECEIVE_PASS:
            benchmarkPrint("PASSED");
            break;
        case BENCHMARK_RECEIVE_PASS_WITH_WRONG_PACKETS:
            benchmarkPrint("PASSED with wrong packets");
            break;
        case BENCHMARK_RECEIVE_FAIL:
            benchmarkPrint("FAILED");
            break;
        default:
            break;
    }
    benchmarkPrint("\n");
    benchmarkPrint("   Expected %u packets\n", pTestCase->mNumberOfPacket);
    benchmarkPrint("   Received %u correct packets\n", pTestCase->mReceiveResult.noOfReceivedPackets);
    benchmarkPrint("   Received %u wrong packets\n", pTestCase->mReceiveResult.noOfWrongPackets);
    if (pTestCase->timeTaken != 0)
    {
        benchmarkPrint("   Time taken: %llu\n", pTestCase->timeTaken);
    }
}

void printSendResult(BenchmarkTestCase_t* pTestCase)
{
    benchmarkPrint("BENCHMARK test case \"%s\" SEND result: ", pTestCase->mTestCaseName);
    switch (pTestCase->mSendResult.verdict)
    {
        case BENCHMARK_SEND_PASS:
            benchmarkPrint("PASSED");
            break;
        case BENCHMARK_SEND_PASS_WITH_MISSED_DEADLINES:
            benchmarkPrint("PASSED with missed deadline(s)");
            break;
        case BENCHMARK_SEND_FAIL:
            benchmarkPrint("FAILED");
            break;      
        default:
            break;
    }
    benchmarkPrint("\n");
    benchmarkPrint("   Tried to send %u packets\n", pTestCase->mNumberOfPacket);
    benchmarkPrint("   Sent %u packets\n", pTestCase->mSendResult.noOfPacketsSent);
    benchmarkPrint("   Missed %u deadlines\n", pTestCase->mSendResult.noOfMissedDeadlines);
    if (pTestCase->timeTaken != 0)
    {
        benchmarkPrint("   Time taken: %llu\n", pTestCase->timeTaken);
    }
}

