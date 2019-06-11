#include "BenchmarkTestCase.h"
#include <windows.h>

unsigned char testBuffer[1000];

BenchmarkTestCase myTestCase("My Test case 1", 100, 100, 100, testBuffer);
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

int main()
{
    BenchmarkTestCase::setSendFunction(&mySendFunction);
    BenchmarkTestCase::setDelayFunction(&mock_delay_function);
    myTestCase.runSend();
    myTestCase.printSendResult();
}
