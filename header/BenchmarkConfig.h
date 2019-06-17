#ifndef BENCHMARKCONFIG_H_
#define BENCHMARKCONFIG_H_

#ifndef nullptr
    #define nullptr NULL
#endif

/* If you are using other print function, route benchmarkPrint to it */
#define benchmarkPrint(...) printf(__VA_ARGS__)

/* The datatype used for time */
typedef unsigned long BenchmarkTime_t;

#endif
