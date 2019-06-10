#include "BenchmarkTestCase.h"

unsigned char mySendFunction(unsigned char* pBuffer, unsigned int size)
{
    return 1;
}

int main()
{
    BenchmarkTestCase::setSendFunction(&mySendFunction);
    BenchmarkTestCase myTestCase("My Test case 1", 0, 0, 0, 0);
    BenchmarkTestCase yourTestCase("My Test case 2", 0, 0, 0, 0);
}