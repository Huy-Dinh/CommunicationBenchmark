#include "BenchmarkTestCase.h"

/* the send function pointer defaults to  */
sendFuncPtr_t BenchmarkTestCase::pSendFunction = nullptr;
delayFuncPtr_t BenchmarkTestCase::pDelayFunction = nullptr;

BenchmarkTestCase::BenchmarkTestCase(std::string testCaseName, unsigned int packetSize, 
                        unsigned int numberOfPacket, unsigned int packetDelay, 
                        unsigned char * dataPointer)
{
    mTestCaseName = testCaseName;
    mPacketSize = packetSize;
    mNumberOfPacket = numberOfPacket;
    mPacketDelay = packetDelay;
    pDataPointer = dataPointer;
}

BenchmarkSendResult_t BenchmarkTestCase::runSend()
{
    /* returns fail if either send or delay callbacks are not registered
        because both are needed for the sending operation to work */
    if (pSendFunction == nullptr || pDelayFunction == nullptr)
        return BENCHMARK_SEND_FAIL;
    
    unsigned int noOfPacketsLeft = mNumberOfPacket;
    while (noOfPacketsLeft > 0)
    {
        /* If an attempt to send fails, the sending test fails */
        if ((*pSendFunction)(pDataPointer, mPacketSize) != BENCHMARK_SEND_PASS)
        {
            return BENCHMARK_SEND_FAIL;
        }
        --noOfPacketsLeft;
        /* Wait to send the next packet */
        (*pDelayFunction)(mPacketDelay);
    }
    /* If we reach here it's assumed that every packets have been put into 
        the buffer successfully */
    return BENCHMARK_SEND_PASS;
}

void BenchmarkTestCase::setSendFunction(sendFuncPtr_t sendFunction)
{
    pSendFunction = sendFunction;
}

void BenchmarkTestCase::setDelayFunction(delayFuncPtr_t delayFunction)
{
    pDelayFunction = delayFunction;
}
