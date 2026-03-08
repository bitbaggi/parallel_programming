MAXIMUM_N_SEQ = 400000000
MAX_SECONDS = 10

from timing_helper import write_timings_to_file
from time_sort_program import run_sort_program

# STRONG SCALING:
THREAD_COUNTS = [8]
BITS_TO_TEST = [8]


for threadCount in THREAD_COUNTS:
    print("Run Aligned_loc program")
    timings = run_sort_program('../problem_4_aligned', BITS_TO_TEST, [MAXIMUM_N_SEQ, MAXIMUM_N_SEQ * 2, MAXIMUM_N_SEQ * 3], MAX_SECONDS,
                               customEnv={'OMP_NUM_THREADS': str(threadCount)})
    write_timings_to_file(f'./results/tests/aligned_{threadCount}_threads.csv', timings)
    print(f"Aligned_loc done")
    print("Run Malloc program")
    timings = run_sort_program('../problem_4_malloc', BITS_TO_TEST, [MAXIMUM_N_SEQ, MAXIMUM_N_SEQ * 2, MAXIMUM_N_SEQ * 3], MAX_SECONDS,
                           customEnv={'OMP_NUM_THREADS': str(threadCount)})
    write_timings_to_file(f'./results/tests/malloc_{threadCount}_threads.csv', timings)
    print(f"Malloc done")

