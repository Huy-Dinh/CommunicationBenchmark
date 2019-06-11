#include <iostream>

/* The result of the sending attempt */
typedef enum 
{
    BENCHMARK_SEND_PASS = 0,
    BENCHMARK_SEND_FAIL
} BenchmarkSendResult_t;

typedef BenchmarkSendResult_t (*sendFuncPtr_t)(unsigned char *, unsigned int);
typedef unsigned char (*receiveFuncPtr_t)(unsigned char *, unsigned int &);
typedef void (*delayFuncPtr_t)(unsigned int);


class BenchmarkTestCase
{
private:
    std::string mTestCaseName;
    unsigned int mPacketSize;
    unsigned int mNumberOfPacket;
    unsigned int mPacketDelay;
    unsigned char * pDataPointer;
    static sendFuncPtr_t pSendFunction;
    static receiveFuncPtr_t pReceiveFunction;
    static delayFuncPtr_t pDelayFunction;
public:
    BenchmarkTestCase(std::string testCaseName, unsigned int packetSize, 
                        unsigned int numberOfPacket, unsigned int packetDelay, 
                        unsigned char * dataPointer);
    BenchmarkSendResult_t runSend();
    static void setSendFunction(sendFuncPtr_t sendFunction);
    static void setDelayFunction(delayFuncPtr_t delayFunction);
    virtual ~BenchmarkTestCase()
    {}
};
