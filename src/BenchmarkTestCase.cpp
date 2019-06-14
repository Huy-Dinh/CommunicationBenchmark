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
    mSendResult.verdict = BENCHMARK_SEND_FAIL;
    mSendResult.noOfMissedDeadlines = 0;
    mReceiveResult = BENCHMARK_RECEIVE_FAIL;
}

BenchmarkSendResult_t BenchmarkTestCase::runSend()
{
    /* returns fail if the callbacks are not registered
        or the sending buffer pointer is not assigned to a valid buffer 
        or the length of the packet is 0*/   
    if (pSendFunction == nullptr || pDelayFunction == nullptr 
        || pGetTickFunction == nullptr || pDataBuffer == nullptr
        || mNumberOfPacket == 0)
    {
        mSendResult.verdict = BENCHMARK_SEND_FAIL;
        return mSendResult;
    }

    unsigned long startTime, endTime, elapsedTime;
    for (unsigned int i = 0; i < mNumberOfPacket - 1; ++i)
    {
        startTime = (*pGetTickFunction)();

        /* If an attempt to send fails, the sending test fails */
        if ((*pSendFunction)(pDataBuffer, mPacketSize) != BENCHMARK_SEND_PASS)
        {
            mSendResult.verdict = BENCHMARK_SEND_FAIL;
            return mSendResult;
        }
        //std::cout << "SEND" << std::endl;
        endTime = (*pGetTickFunction)();
        elapsedTime = endTime - startTime;
        /* If it met the deadline */
        if (elapsedTime <= mPacketDelay)
        {
            /* Wait to send the next packet */
            (*pDelayFunction)(mPacketDelay - elapsedTime);
        }
        else
        {
            /* Increase the dealine counter since a deadline was missed */
            ++(mSendResult.noOfMissedDeadlines);
        }
        
    }
    /* Send the last packet without a delay */
    if ((*pSendFunction)(pDataBuffer, mPacketSize) != BENCHMARK_SEND_PASS)
    {
        mSendResult.verdict = BENCHMARK_SEND_FAIL;
        return mSendResult;
    }
    /* If we reach here it's assumed that every packets have been put into 
        the buffer successfully, only the number of missed deadlines need to be checked */
    mSendResult.verdict = (mSendResult.noOfMissedDeadlines == 0) ? BENCHMARK_SEND_PASS : BENCHMARK_SEND_PASS_WITH_MISSED_DEADLINES;
    return mSendResult;
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
    if (mSendResult.verdict == BENCHMARK_SEND_PASS)
    {
        std::cout << "PASSED";
    }
    else if (mSendResult.verdict == BENCHMARK_SEND_PASS_WITH_MISSED_DEADLINES)
    {
        std::cout << "PASSED with " << mSendResult.noOfMissedDeadlines << " missed deadlines";
    }
    else
    {
        std::cout << "FAILED";
    }
    std::cout << std::endl;
#endif 
}
