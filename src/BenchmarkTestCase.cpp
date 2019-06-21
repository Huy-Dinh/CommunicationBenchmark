#include "BenchmarkTestCase.h"

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
    mPacketDelay = packetDelay;
    pDataBuffer = dataPointer;

    mSendResult.verdict = BENCHMARK_SEND_FAIL;
    mSendResult.noOfPacketsSent = 0;
    mSendResult.noOfMissedDeadlines = 0;

    mReceiveResult.verdict = BENCHMARK_RECEIVE_FAIL;
    mReceiveResult.noOfReceivedPackets = 0;
    mReceiveResult.noOfWrongPackets = 0;
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
        /* Increase the counter for successfully sent bytes */
        ++mSendResult.noOfPacketsSent;

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
            /* Increase the missed deadline counter since a deadline was missed */
            ++(mSendResult.noOfMissedDeadlines);
        }
    }
    /* Send the last packet without a delay */
    if ((*pSendFunction)(pDataBuffer, mPacketSize) != BENCHMARK_SEND_PASS)
    {
        mSendResult.verdict = BENCHMARK_SEND_FAIL;
        return mSendResult;
    }
    ++mSendResult.noOfPacketsSent;
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
        ++mReceiveResult.noOfWrongPackets;
        return BENCHMARK_PACKET_CHECK_FAIL;
    }
    /* compare the received buffer */
    if (memcmp(pBuffer, pDataBuffer, mPacketSize) != 0)
    {
        ++mReceiveResult.noOfWrongPackets;
        return BENCHMARK_PACKET_CHECK_FAIL;
    }
    /* if all the packets have been received correctly, the test case passed */
    if (++mReceiveResult.noOfReceivedPackets == mNumberOfPacket)
    {
        mReceiveResult.verdict = (mReceiveResult.noOfWrongPackets == 0) ? BENCHMARK_RECEIVE_PASS : BENCHMARK_RECEIVE_PASS_WITH_WRONG_PACKETS;
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
    benchmarkPrint("BENCHMARK test case \"%s\" RECEIVE result: ", mTestCaseName.c_str());
    switch (mReceiveResult.verdict)
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
    benchmarkPrint("   Expected %u packets\n", mNumberOfPacket);
    benchmarkPrint("   Received %u correct packets\n", mReceiveResult.noOfReceivedPackets);
    benchmarkPrint("   Received %u wrong packets\n", mReceiveResult.noOfWrongPackets);
}

void BenchmarkTestCase::printSendResult()
{
    benchmarkPrint("BENCHMARK test case \"%s\" SEND result: ", mTestCaseName.c_str());
    switch (mSendResult.verdict)
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
    benchmarkPrint("   Tried to send %u packets\n", mNumberOfPacket);
    benchmarkPrint("   Sent %u packets\n", mSendResult.noOfPacketsSent);
    benchmarkPrint("   Missed %u deadlines\n", mSendResult.noOfMissedDeadlines);
}
