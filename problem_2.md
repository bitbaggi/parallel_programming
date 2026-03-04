# Problem 2: experiments with my algorithm

# 16 Bits is maximum on most machines 
From problem 1, I know that bits over 16 is not possible, bcs.
the next number would be 32 bits
When we create buckets for 32 bits, we create 2^32 buckets
2^32 buckets are = 4.294.967.296 buckets
1 bucket needs 4 bytes for an int
In total: 4 bytes * 4.294.967.296 = 17,18 GB
Could be possible on some machines but definitely not a good approach.
with 64 bits its like 73.786,98 PB needed Memory -> impossible at the moment.

# 3 arrays to store data are to much
705032704 numbers are ~5,6 GB
with 3 lists then 3 arrays it would be ~16,8 GB of reserved space

# python program to test b/n combinations and draw graphs and stuff

