#include "BenchmarkTestCase.h"
#include <windows.h>
#include <stdio.h>

unsigned char testBuffer[1000];

BenchmarkTestCase myTestCase("My Test case 1", 1000, 10, 1000000, testBuffer);
BenchmarkTestCase yourTestCase("My Test case 2", 50, 100, 10, testBuffer);

void mock_receive_callback(unsigned char* pBuffer, unsigned int size)
{
    if (BENCHMARK_PACKET_CHECK_PASS_FINISH == myTestCase.checkReceivedPacket(pBuffer, size))
    {
        myTestCase.printReceiveResult();
    }
}

BenchmarkSendResult_t mySendFunction(unsigned char* pBuffer, unsigned int size)
{
    mock_receive_callback(pBuffer, size);
    return BENCHMARK_SEND_PASS;
}

/* Time ticks */

LARGE_INTEGER nFrequency;
void getFrequency()
{
    QueryPerformanceFrequency(&nFrequency);
}

unsigned long getMicroseconds()
{
    LARGE_INTEGER currentTickCount;
    QueryPerformanceCounter(&currentTickCount);
    currentTickCount.QuadPart *= 1000000;
    currentTickCount.QuadPart /= nFrequency.QuadPart;
    return (unsigned long) currentTickCount.QuadPart;
}

void mock_delay_function(unsigned long delayTime)
{
    unsigned long startTime = getMicroseconds();
    while (getMicroseconds() - startTime < delayTime);
}

int main()
{
    getFrequency();
    BenchmarkTestCase::setSendFunction(&mySendFunction);
    BenchmarkTestCase::setDelayFunction(&mock_delay_function);
    BenchmarkTestCase::setGetTickFunction(&getMicroseconds);
    myTestCase.runSend();
    myTestCase.printSendResult();
}

