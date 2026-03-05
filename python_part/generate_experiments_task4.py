MAXIMUM_N_SEQ = 470000000
MAX_SECONDS = 10

from timing_helper import write_timings_to_file
from time_sort_program import run_sort_program

# STRONG SCALING:
THREAD_COUNTS = [1, 2, 4, 8, 16, 32]
BITS_TO_TEST = [8]
N_TO_TEST = lambda p: {n for n in range(MAXIMUM_N_SEQ, MAXIMUM_N_SEQ * p + 1 , MAXIMUM_N_SEQ)}

print(N_TO_TEST(1))
for threadCount in THREAD_COUNTS:
    print("Run sort program")
    timings = run_sort_program('../problem_4', BITS_TO_TEST, N_TO_TEST(threadCount), MAX_SECONDS,
                               customEnv={'OMP_NUM_THREADS': str(threadCount)})
    write_timings_to_file(f'./results/timings_parallel_{threadCount}_threads.csv', timings)
    print(f"ThreadCount {threadCount} done")
