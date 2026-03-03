import subprocess
import re

MAX_SECONDS = 10
BIT_COUNTS_TO_TEST = [2, 4, 8, 16]
max_n_by_bits = {}

for bits in BIT_COUNTS_TO_TEST:
    for numberCount in range(10000, 10000000000, 50000000):
        output = subprocess.check_output(["../problem_1", f"{numberCount} {bits}"])
        time = float(re.search(r"Average time taken: (\d+\.\d+) seconds", output.decode()).group(1))
        print(f"Time for combination n={numberCount}, b={bits} is {time}s")
        if time < MAX_SECONDS:
            max_n_by_bits[bits] = numberCount
        else:
            break


print("\n\nMax n in 10 sec for each bit count:")
for bits, max_n in max_n_by_bits.items():
    print(f"Bits: {bits}, Max n: {max_n}")