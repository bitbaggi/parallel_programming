//
// Copyright (c) 2026 Pascal Keßler - All rights reserved.
//

// Needed to use qsort to check if the radix sort works correctly
#include <stdlib.h>
#include "mt19937-64.c"
#include <omp.h>
#include <string.h>

#define NUMBER_OF_RUNS 10

/**
 * Comparator for the qsort function, just for check if the radix sort works correctly
 * @param a
 * @param b
 * @return
 */
int compare(const void* a, const void* b)
{
    const unsigned long ua = *(unsigned long*)a;
    const unsigned long ub = *(unsigned long*)b;
    if (ua < ub) return -1;
    if (ua > ub) return 1;
    return 0;
}

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
double sortArray_withRadixSort(unsigned long numbersToSort[], unsigned long numbersToSwap[], const long n, const int b)
{
    const long numberOfBuckets = 1 << b;
    int* bucketSizes = malloc(numberOfBuckets * sizeof(int));
    int* bucketStart = malloc(numberOfBuckets * sizeof(int));

    const double start_time = omp_get_wtime();
    for (int bitrange = 0; bitrange < 64; bitrange += b)
    {
        memset(bucketSizes, 0, numberOfBuckets * sizeof(int));
        memset(bucketStart, 0, numberOfBuckets * sizeof(int));

        const unsigned long bitMaskForBitrange = (1UL << b) - 1;
        for (long i = 0; i < n; i += 1)
        {
            bucketSizes[numbersToSort[i] >> bitrange & bitMaskForBitrange]++;
        }
        for (long i = 1; i < numberOfBuckets; i++)
        {
            bucketStart[i] = bucketStart[i - 1] + bucketSizes[i - 1];
        }
        for (long i = 0; i < n; i += 1)
        {
            numbersToSwap[
                bucketStart[numbersToSort[i] >> bitrange & bitMaskForBitrange]++
            ] = numbersToSort[i];
        }

        unsigned long* temp = numbersToSort;
        numbersToSort = numbersToSwap;
        numbersToSwap = temp;
    }
    const double timing = omp_get_wtime() - start_time;

    free(bucketSizes);
    free(bucketStart);
    return timing;
}


int main(int argc, char* argv[])
{
    const long n = argc > 1 ? atoi(argv[1]) : 100000;
    const int b = argc > 2 ? atoi(argv[2]) : 8;
    if (b == 1 || !isPowerOfTwo(b))
    {
        printf("Please insert b, so b is power of 2\n");
        return 1;
    }
    if (b > 32)
    {
        printf("Please insert b, so b is smaller than or equal to 16\n");
        return 1;
    }

    unsigned long* numbersToSort = aligned_alloc(64, n * sizeof(unsigned long));
    unsigned long* numbersToSwap = aligned_alloc(64, n * sizeof(unsigned long));
    unsigned long* numbersSorted = aligned_alloc(64, n * sizeof(unsigned long));

    double totalTime = 0;
    for (int i = 0; i < NUMBER_OF_RUNS; i++)
    {
        for (long g = 0; g < n; g++)
        {
            numbersToSort[g] = genrand64_int64();
            numbersSorted[g] = numbersToSort[g];
        }

        totalTime += sortArray_withRadixSort(numbersToSort, numbersToSwap, n, b);

        qsort(numbersSorted, n, sizeof(unsigned long), compare);
        for (long j = 0; j < n; j++)
        {
            if (numbersSorted[j] == numbersToSort[j]) continue;
            printf("Error at index %lu: expected %lu, got %lu\n", j, numbersSorted[j], numbersToSort[j]);
            return 1;
        }
    }
    printf("Average time taken: %f seconds\n", totalTime / NUMBER_OF_RUNS);

    free(numbersToSort);
    free(numbersToSwap);
    free(numbersSorted);

    return 0;
}
