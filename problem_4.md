# Problem 2: experiments with my algorithm

# 64gb memory is not enough
because of the 3 arrays with n * 64 bit the program needs
n = 2350000000, 2350000000 * 8 bytes = 18,8 gb => 3 * 18,8 = 56,4 gb

-> remove extra list numbers-initial


# cache lines
allocate bucketsizes as continuous memory lines


# l1 vs l2 cache
l1 cache is 32kb
-> b=8 => 256 buckets á 
l2 cache is 256kb