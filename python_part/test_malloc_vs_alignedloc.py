MAXIMUM_N_SEQ = 400000000
MAX_SECONDS = 10

from timing_helper import write_timings_to_file
from time_sort_program import run_sort_program

# STRONG SCALING:
BITS_TO_TEST = [8]

print("Run sort program")
timings = run_sort_program('../problem_4_aligned', BITS_TO_TEST, [MAXIMUM_N_SEQ], MAX_SECONDS,
                           customEnv={'OMP_NUM_THREADS': str(8)})
write_timings_to_file(f'./results/tests/aligned_threads.csv', timings)
print(f"Aligned_loc done")

print("Run sort program")
timings = run_sort_program('../problem_4_malloc', BITS_TO_TEST, [MAXIMUM_N_SEQ], MAX_SECONDS,
                           customEnv={'OMP_NUM_THREADS': str(8)})
write_timings_to_file(f'./results/tests/malloc_threads.csv', timings)
print(f"Malloc done")