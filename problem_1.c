//
// Copyright (c) 2026 Pascal Keßler - All rights reserved.
//

#define TIMING

#define NUMBER_OF_RUNS 5
#define RANDOM_SEED 1234ULL

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

/**
 *
 * @param numbersToSort as unsigned long values
 * @param n number of elements to sort
 * @param b number of bits per element
 */
double sortArray_withRadixSort_sequential(unsigned long numbersToSort[], unsigned long numbersToSwap[], const long n, const int b)
{
    const int numberOfBuckets = 1 << b;
    int* bucketSizes = malloc(numberOfBuckets * sizeof(int));
    int* bucketStart = malloc(numberOfBuckets * sizeof(int));

    const double start_time = omp_get_wtime();
    for (int bitrange = 0; bitrange < 64; bitrange += b)
    {
        memset(bucketSizes, 0, numberOfBuckets * sizeof(int));
        memset(bucketStart, 0, numberOfBuckets * sizeof(int));

        const unsigned long bitMaskForBitrange = (1UL << b) - 1;
#ifdef TIMING
        const double start_time_bucket_size_calc = omp_get_wtime();
#endif
        for (long i = 0; i < n; i += 1)
        {
            bucketSizes[numbersToSort[i] >> bitrange & bitMaskForBitrange]++;
        }
#ifdef TIMING
        timings[numberOfRun][0] += omp_get_wtime() - start_time_bucket_size_calc;
#endif


#ifdef TIMING
        const double start_time_startpoint_calc = omp_get_wtime();
#endif
        for (long i = 1; i < numberOfBuckets; i++)
        {
            bucketStart[i] = bucketStart[i - 1] + bucketSizes[i - 1];
        }
#ifdef TIMING
        timings[numberOfRun][1] += omp_get_wtime() - start_time_startpoint_calc;
#endif

#ifdef TIMING
        const double start_time_bucket_insertion = omp_get_wtime();
#endif
        for (long i = 0; i < n; i += 1)
        {
            numbersToSwap[
                bucketStart[numbersToSort[i] >> bitrange & bitMaskForBitrange]++
            ] = numbersToSort[i];
        }
#ifdef TIMING
        timings[numberOfRun][2] += omp_get_wtime() - start_time_bucket_insertion;
#endif

        unsigned long* temp = numbersToSort;
        numbersToSort = numbersToSwap;
        numbersToSwap = temp;
    }
    const double totalTime = omp_get_wtime() - start_time;
#ifdef TIMING
    timings[numberOfRun][3] = totalTime;
#endif

    free(bucketSizes);
    free(bucketStart);
    return totalTime;
}

int main(int argc, char* argv[])
{
    const long n = argc > 1 ? atoi(argv[1]) : 100000;
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

    unsigned long* numbersInitial = aligned_alloc(64, n * sizeof(unsigned long));

    init_genrand64(RANDOM_SEED);
    for (long g = 0; g < n; g++)
    {
        numbersInitial[g] = genrand64_int64();
    }

    unsigned long* numbersToSort = aligned_alloc(64, n * sizeof(unsigned long));
    unsigned long* numbersToSwap = aligned_alloc(64, n * sizeof(unsigned long));
    // Initialize the arrays to zero to avoid any potential issues with uninitialized memory pages during timing
    memset(numbersToSort, 0, n * sizeof(unsigned long));
    memset(numbersToSwap, 0, n * sizeof(unsigned long));

    double totalTime = 0;
    for (numberOfRun = 0; numberOfRun < NUMBER_OF_RUNS; numberOfRun++)
    {
        memcpy(numbersToSort, numbersInitial, n * sizeof(unsigned long));

        totalTime += sortArray_withRadixSort_sequential(numbersToSort, numbersToSwap, n, b);
        if (!is_sorted(numbersToSort, n))
        {
            printf("Failed to sort");
            return 1;
        }
    }


#ifdef TIMING
    printf("Timing result of sorting: [bucketsize calc,            bucketstart calc,          bucket filling]\n");
    double accumulated_timings[4];
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

    free(numbersToSort);
    free(numbersToSwap);
    return 0;
}
