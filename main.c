#include "BenchmarkTestCase.h"
#include "BenchmarkSender.h"
#include "BenchmarkReceiver.h"
#include <windows.h>

unsigned char testBuffer[1000];

#define NUMBER_OF_TESTCASES 3
BenchmarkTestCase_t testCases[NUMBER_OF_TESTCASES] =
{
    {"My Test case 1", 1, 100000, 10, testBuffer},
    {"My Test case 2", 100, 100000, 10, testBuffer},
    {"My Test case 3", 1000, 100000, 10, testBuffer}
};

BenchmarkTestCase_t sendTestCases[NUMBER_OF_TESTCASES] =
{
    {"My Test case 1", 1, 100000, 10, testBuffer},
    {"My Test case 2", 100, 100000, 10, testBuffer},
    {"My Test case 3", 1000, 100000, 10, testBuffer}
};

BenchmarkReceiver_t myReceiver;
BenchmarkSender_t mySender;

void mock_receive_callback(unsigned char* pBuffer, unsigned int size)
{
    benchmarkReceive(&myReceiver, pBuffer, size);
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

    mySender.mTestCases = sendTestCases;
    mySender.mNumberOfTestCases = NUMBER_OF_TESTCASES;
    mySender.pSendFunction = mySendFunction;
    mySender.pDelayFunction = mock_delay_function;
    mySender.pGetTickFunction = getMicroseconds;
    

    myReceiver.mNumberOfTestCases = NUMBER_OF_TESTCASES;
    myReceiver.mTestCases = testCases;
    myReceiver.mCurrentTestCase = 0;
    myReceiver.mState = RECEIVER_STATE_IDLE;
    
    runSend(&mySender);
}

