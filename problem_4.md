# Problem 2: experiments with my algorithm

# 64gb memory is not enough
because of the 3 arrays with n * 64 bit the program needs
n = 2350000000, 2350000000 * 8 bytes = 18,8 gb => 3 * 18,8 = 56,4 gb

-> remove extra list numbers-initial


# cache lines
allocate bucketsizes as continuous memory lines
