#include "BenchmarkTestCase.h"
#include <windows.h>
#include <stdio.h>

unsigned char testBuffer[1000];

BenchmarkTestCase myTestCase("My Test case 1", 1000, 10, 1000, testBuffer);
BenchmarkTestCase yourTestCase("My Test case 2", 50, 100, 10, testBuffer);

void mock_delay_function(unsigned int delayTime)
{
    Sleep(delayTime);
}

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

unsigned long getMilliseconds()
{
    LARGE_INTEGER currentTickCount;
    QueryPerformanceCounter(&currentTickCount);
    currentTickCount.QuadPart *= 1000;
    currentTickCount.QuadPart /= nFrequency.QuadPart;
    return (unsigned long) currentTickCount.QuadPart;
}

int main()
{
    getFrequency();
    BenchmarkTestCase::setSendFunction(&mySendFunction);
    BenchmarkTestCase::setDelayFunction(&mock_delay_function);
    BenchmarkTestCase::setGetTickFunction(&getMilliseconds);
    myTestCase.runSend();
    myTestCase.printSendResult();
}

