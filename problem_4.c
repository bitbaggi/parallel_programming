//
// Copyright (c) 2026 Pascal Keßler - All rights reserved.
//

#define TIMING

#define NUMBER_OF_RUNS 5
#define RANDOM_SEED 1234ULL
#define CACHE_LINE 64

#include <omp.h>
#include "mt19937-64.c"
// malloc
#include <stdlib.h>
// memset
#include <string.h>

#ifdef TIMING
#include <math.h>
#endif


int numberOfRun = 0;
#ifdef TIMING
double timings[NUMBER_OF_RUNS][4];
int threadsUsed[NUMBER_OF_RUNS][64];
#endif

int is_sorted(const unsigned long* numbers, const long n)
{
    for (long i = 1; i < n; i++)
    {
        if (numbers[i] < numbers[i - 1])
        {
            printf("Fehler bei Index %zu: %lu > %lu\n", i, numbers[i - 1], numbers[i]);
            return 0;
        }
    }
    return 1;
}

#ifdef TIMING
double sumOfSubarray(double** numbers, int g)
{
    double total = 0;
    for (int i = 0; i < 4; i++)
    {
        total += numbers[i][g];
    }
    return total;
}
#endif

int isPowerOfTwo(const int b)
{
    return b > 0 && (b & b - 1) == 0;
}

double sortArray_withRadixSort_parallel(unsigned long numbersToSort[], unsigned long numbersToSwap[], const long n,
                                        const int b)
{
    const double startTime = omp_get_wtime();

    const int numBuckets = 1 << b;

    int numThreads = omp_get_max_threads();
    int** threadBucketSize = malloc(numThreads * sizeof(int*));
    long** threadStartingPoint = malloc(numThreads * sizeof(long*));
    for (int t = 0; t < numThreads; t++)
    {
        threadBucketSize[t] = aligned_alloc(CACHE_LINE, numBuckets * sizeof(int));
        threadStartingPoint[t] = aligned_alloc(CACHE_LINE, numBuckets * sizeof(long));
    }

    for (int bitrange = 0; bitrange < 64; bitrange += b)
    {
        const unsigned long bitMaskForBitrange = (1UL << b) - 1;
        double timingsBucketSizeCalculation = 0;
        double timingsBucketInsertions = 0;
#pragma omp parallel reduction(+:timingsBucketSizeCalculation)  reduction(+:timingsBucketInsertions)
        {
            const int tid = omp_get_thread_num();

#pragma omp single
            {
                numThreads = omp_get_num_threads();
#ifdef TIMING
                threadsUsed[numberOfRun][bitrange] = numThreads;
#endif
            }

            memset(threadBucketSize[tid], 0, numBuckets * sizeof(int));

            long i;
            double startTimeBucketSizeCalculation = omp_get_wtime();
#pragma omp for schedule(static) private(i)
            for (i = 0; i < n; i++)
                threadBucketSize[tid][numbersToSort[i] >> bitrange & bitMaskForBitrange]++;
            timingsBucketSizeCalculation += omp_get_wtime() - startTimeBucketSizeCalculation;

#pragma omp single
            {
                double startTimeBucketStartCalculation = omp_get_wtime();
                long startIdxInCompleteNumberList = 0;
                for (int bucketNum = 0; bucketNum < numBuckets; bucketNum++)
                {
                    for (int threadNum = 0; threadNum < numThreads; threadNum++)
                    {
                        threadStartingPoint[threadNum][bucketNum] = startIdxInCompleteNumberList;
                        startIdxInCompleteNumberList += threadBucketSize[threadNum][bucketNum];
                    }
                }
                timings[numberOfRun][1] += omp_get_wtime() - startTimeBucketStartCalculation;
            }


            const double startTimeBucketInsertion = omp_get_wtime();
#pragma omp for schedule(static) private(i)
            for (i = 0; i < n; i++)
            {
                numbersToSwap[threadStartingPoint[tid][
                        numbersToSort[i] >> bitrange & bitMaskForBitrange]++
                ] = numbersToSort[i];
            }
            timingsBucketInsertions += omp_get_wtime() - startTimeBucketInsertion;
        }

        timings[numberOfRun][0] += timingsBucketSizeCalculation / numThreads;
        timings[numberOfRun][2] += timingsBucketInsertions / numThreads;

        unsigned long* temp = numbersToSort;
        numbersToSort = numbersToSwap;
        numbersToSwap = temp;
    }
    const double totalTime = omp_get_wtime() - startTime;
#ifdef TIMING
    timings[numberOfRun][3] = totalTime;
#endif
    for (int t = 0; t < numThreads; t++)
    {
        free(threadBucketSize[t]);
        free(threadStartingPoint[t]);
    }

    free(threadBucketSize);
    free(threadStartingPoint);
    return totalTime;
}

int main(int argc, char* argv[])
{
    const long n = argc > 1 ? atol(argv[1]) : 100000;
    const int b = argc > 2 ? atoi(argv[2]) : 8;
    if (!isPowerOfTwo(b))
    {
        printf("Please insert b, so b is power of 2\n");
        return 1;
    }
    if (b > 16)
    {
        printf("Please insert b, so b is smaller than or equal to 16\n");
        return 1;
    }

    printf("Run sort by radix sort (n: %lu, b: %d)\n", n, b);
    unsigned long* numbersToSort = aligned_alloc(CACHE_LINE, n * sizeof(unsigned long));
    unsigned long* numbersToSwap = aligned_alloc(CACHE_LINE, n * sizeof(unsigned long));

    init_genrand64(RANDOM_SEED);

    double totalTime = 0;
    for (numberOfRun = 0; numberOfRun < NUMBER_OF_RUNS; numberOfRun++)
    {
#pragma parallel for
        for (long i = 0; i < n; i++)
        {
            numbersToSort[i] = genrand64_int64();
            numbersToSwap[i] = 0;
        }

        totalTime += sortArray_withRadixSort_parallel(numbersToSort, numbersToSwap, n, b);
        if (!is_sorted(numbersToSort, n))
        {
            printf("Failed to sort");
            return 1;
        }
    }


#ifdef TIMING
    printf("Timing result of sorting: [bucketsize calc,            bucketstart calc,          bucket filling]\n");
    double accumulated_timings[4] = {0};
    for (int i = 0; i < NUMBER_OF_RUNS; i++)
    {
        printf("Detailed Timing of Run %d: ", i + 1);
        printf("[%.8f (%.8f %%), %.8f (%.8f %%), %.8f (%.8f %%)]\n",
               timings[i][0], timings[i][0] / timings[i][3] * 100,
               timings[i][1], timings[i][1] / timings[i][3] * 100,
               timings[i][2], timings[i][2] / timings[i][3] * 100);
        accumulated_timings[0] += timings[i][0];
        accumulated_timings[1] += timings[i][1];
        accumulated_timings[2] += timings[i][2];
        accumulated_timings[3] += timings[i][3];
    }
    printf("Average Time of the Runs: ");
    printf("[%.8f (%.8f %%), %.8f (%.8f %%), %.8f (%.8f %%)]\n",
           accumulated_timings[0] / NUMBER_OF_RUNS, accumulated_timings[0] / accumulated_timings[3] * 100,
           accumulated_timings[1] / NUMBER_OF_RUNS, accumulated_timings[1] / accumulated_timings[3] * 100,
           accumulated_timings[2] / NUMBER_OF_RUNS, accumulated_timings[2] / accumulated_timings[3] * 100);
#endif
    printf("Average time taken: %f seconds\n", totalTime / NUMBER_OF_RUNS);

#ifdef TIMING
    printf("Threads really used: [");
    for (int i = 0; i < NUMBER_OF_RUNS; i++)
    {
        printf("Run %d: (", i + 1);
        for (int bitrange = 0; bitrange < 63; bitrange += b)
        {
            printf("%d", threadsUsed[i][bitrange]);
            if (bitrange < 63 - b) printf(", ");
        }
        printf(")");
        if (i != NUMBER_OF_RUNS - 1) printf(", ");
    }
    printf("]\n");
#endif


    free(numbersToSort);
    free(numbersToSwap);
    return 0;
}
