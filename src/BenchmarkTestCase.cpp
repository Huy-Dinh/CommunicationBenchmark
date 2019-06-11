#include "BenchmarkTestCase.h"
#include <string.h>
#include <stdio.h>

/* the send function pointer defaults to  */
sendFuncPtr_t BenchmarkTestCase::pSendFunction = nullptr;
delayFuncPtr_t BenchmarkTestCase::pDelayFunction = nullptr;
getTickFuncPtr_t BenchmarkTestCase::pGetTickFunction = nullptr;

BenchmarkTestCase::BenchmarkTestCase(std::string testCaseName, unsigned int packetSize, 
                        unsigned int numberOfPacket, unsigned int packetDelay, 
                        unsigned char * dataPointer)
{
    mTestCaseName = testCaseName;
    mPacketSize = packetSize;
    mNumberOfPacket = numberOfPacket;
    mExpectedNoOfPacket = mNumberOfPacket;
    mPacketDelay = packetDelay;
    pDataBuffer = dataPointer;
    mSendResult = BENCHMARK_SEND_FAIL;
    mReceiveResult = BENCHMARK_RECEIVE_FAIL;
}

BenchmarkSendResult_t BenchmarkTestCase::runSend()
{
    /* returns fail if the callbacks are not registered
        because all are needed for the sending operation to work
        or if the sending buffer pointer is not assigned to a 
        valid buffer */   
    if (pSendFunction == nullptr || pDelayFunction == nullptr 
        || pGetTickFunction == nullptr || pDataBuffer == nullptr)
    {
        mSendResult = BENCHMARK_SEND_FAIL;
        return BENCHMARK_SEND_FAIL;
    }

    unsigned long startTime, endTime, elapsedTime;
    for (unsigned int i = 0; i < mNumberOfPacket; ++i)
    {
        startTime = (*pGetTickFunction)();

        /* If an attempt to send fails, the sending test fails */
        if ((*pSendFunction)(pDataBuffer, mPacketSize) != BENCHMARK_SEND_PASS)
        {
            mSendResult = BENCHMARK_SEND_FAIL;
            return BENCHMARK_SEND_FAIL;
        }
        //std::cout << "SEND" << std::endl;
        endTime = (*pGetTickFunction)();
        elapsedTime = endTime - startTime;
        if (elapsedTime < mPacketDelay)
        {
            /* Wait to send the next packet */
            (*pDelayFunction)(mPacketDelay - elapsedTime);
        }
    }
    /* If we reach here it's assumed that every packets have been put into 
        the buffer successfully */
    mSendResult = BENCHMARK_SEND_PASS;
    return BENCHMARK_SEND_PASS;
}

BenchmarkPacketCheckResult_t BenchmarkTestCase::checkReceivedPacket(unsigned char * pBuffer, unsigned int length)
{
     /* if the buffer pointer is not assigned to a valid buffer*/
    if (pDataBuffer == nullptr)
    {
        return BENCHMARK_PACKET_CHECK_FAIL;
    }
    /* if the received packet length is different from the one specified in the test case
        it's a fail */
    if (length != mPacketSize)
    {
        return BENCHMARK_PACKET_CHECK_FAIL;
    }
    /* compare the received buffer */
    if (memcmp(pBuffer, pDataBuffer, mPacketSize) != 0)
    {
        return BENCHMARK_PACKET_CHECK_FAIL;
    }
    /* if all the packets have been received correctly, the test case passed */
    if (--mExpectedNoOfPacket == 0)
    {
        mReceiveResult = BENCHMARK_RECEIVE_PASS;
        return BENCHMARK_PACKET_CHECK_PASS_FINISH;
    }
    return BENCHMARK_PACKET_CHECK_PASS;
}

void BenchmarkTestCase::setSendFunction(sendFuncPtr_t sendFunction)
{
    pSendFunction = sendFunction;
}

void BenchmarkTestCase::setDelayFunction(delayFuncPtr_t delayFunction)
{
    pDelayFunction = delayFunction;
}

void BenchmarkTestCase::setGetTickFunction(getTickFuncPtr_t getTickFunction)
{
    pGetTickFunction = getTickFunction;
}


void BenchmarkTestCase::printReceiveResult()
{
#ifdef STDOUT_RESULT
    std::cout << "Test case: " << mTestCaseName;
    std::cout << " receive result: ";
    if (mReceiveResult == BENCHMARK_RECEIVE_PASS)
    {
        std::cout << "PASSED";
    }
    else
    {
        std::cout << "FAILED";
    }
    std::cout << std::endl;
#endif 
}
void BenchmarkTestCase::printSendResult()
{
#ifdef STDOUT_RESULT
    std::cout << "Test case: " << mTestCaseName;
    std::cout << " send result: ";
    if (mSendResult == BENCHMARK_SEND_PASS)
    {
        std::cout << "PASSED";
    }
    else
    {
        std::cout << "FAILED";
    }
    std::cout << std::endl;
#endif 
}
