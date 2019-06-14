#include "BenchmarkTestCase.h"
#include "BenchmarkSender.h"
#include "BenchmarkReceiver.h"
#include <windows.h>
#include <stdio.h>

unsigned char testBuffer[1000];

#define NUMBER_OF_TESTCASES 3
BenchmarkTestCase testCases[NUMBER_OF_TESTCASES] =
{
    BenchmarkTestCase("My Test case 1", 1, 100000, 10, testBuffer),
    BenchmarkTestCase("My Test case 2", 100, 100000, 10, testBuffer),
    BenchmarkTestCase("My Test case 3", 1000, 100000, 10, testBuffer)
};

BenchmarkTestCase sendTestCases[NUMBER_OF_TESTCASES] =
{
    BenchmarkTestCase("My Test case 1", 1, 50, 10, testBuffer),
    BenchmarkTestCase("My Test case 2", 100, 100000, 10, testBuffer),
    BenchmarkTestCase("My Test case 3", 1000, 100000, 10, testBuffer)
};


BenchmarkReceiver myReceiver(testCases, NUMBER_OF_TESTCASES);

void mock_receive_callback(unsigned char* pBuffer, unsigned int size)
{
    myReceiver.receive(pBuffer, size);
}

BenchmarkSendVerdict_t mySendFunction(unsigned char* pBuffer, unsigned int size)
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
    BenchmarkSender mySender(sendTestCases, NUMBER_OF_TESTCASES, &mySendFunction, &mock_delay_function, &getMicroseconds, 2000000);
    mySender.runSend();
}

