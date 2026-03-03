# Problem 1: Sequential radix sort

Pseudocode:
    init arrays numbers and numbersSwap with size n to hold numbers
    loop over 64 bits in steps with b bits per step:
        calculate number of buckets by bitshift b so we get 2^b
        init arrays to store bucketSizes and startpoint of each bucket
        loop over buckets
            reset values of bucket arrays to 0
        loop over n:
            increase bucketSize for the b bits of each number 
        loop over buckets
            calculate startpoint for each bucket
        loop over n:
            put each number in its right bucket
        swap numbers with numbersSwap
    -> numbers is now sorted(, if we have odd number of loops then it should be numbersSwap)

O-Notation:
O(64/b)
loop over 64 bits in steps with b bits per step:
    O(1)
    calculate number of buckets by bitshift b so we get 2^b
    O(1)
    init arrays to store bucketSizes and startpoint of each bucket
    O(2^b)
    loop over buckets
        reset values of bucket arrays to 0
    O(n)
    loop over n:
        increase bucketSize for the b bits of each number
    O(2^b)
    loop over buckets
        calculate startpoint for each bucket
    O(n)
    loop over n:
        put each number in its right bucket
    O(1)
    swap numbers with numbersSwap

t_s = O(64/b * (2*2^b + 2n))