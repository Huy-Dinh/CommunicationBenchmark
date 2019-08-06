#ifndef BENCHMARKCONFIG_H_
#define BENCHMARKCONFIG_H_

#ifndef NULL
    #define NULL 0
#endif

#ifndef nullptr
    #define nullptr NULL
#endif

/* Number of maximum failures  */
#define BENCHMARK_MAX_SEND_FAILURE  1000

/* Printing configuration */
#define BENCHMARK_ENABLE_PRINT      1
#define BENCHMARK_PRINT_FUNCTION    printf // Route to other print function if necessary

/* The datatype used for time */
typedef unsigned long BenchmarkTime_t;

#if BENCHMARK_ENABLE_PRINT
    #define benchmarkPrint(...) BENCHMARK_PRINT_FUNCTION(__VA_ARGS__)
#else
    #define benchmarkPrint(...) {}
#endif

#endif
