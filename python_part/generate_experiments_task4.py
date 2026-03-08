MAXIMUM_N_SEQ = 400000000
MAX_SECONDS = 10

from timing_helper import write_timings_to_file
from time_sort_program import run_sort_program

# STRONG SCALING:
THREAD_COUNTS = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
BITS_TO_TEST = [8]

for threadCount in THREAD_COUNTS:
    print("Run sort program")
    timings = run_sort_program('../problem_4', BITS_TO_TEST, [MAXIMUM_N_SEQ], MAX_SECONDS,
                               customEnv={'OMP_NUM_THREADS': str(threadCount)})
    write_timings_to_file(f'./results/parallel/strong_scaling/{threadCount}_threads.csv', timings)
    print(f"ThreadCount {threadCount} done")

# WEAK SCALING:
for threadCount in THREAD_COUNTS:
    print("Run sort program")
    timings = run_sort_program('../problem_4', BITS_TO_TEST, [MAXIMUM_N_SEQ * threadCount], MAX_SECONDS,
                               customEnv={'OMP_NUM_THREADS': str(threadCount)})
    write_timings_to_file(f'./results/parallel/weak_scaling/{threadCount}_threads.csv', timings)
    print(f"ThreadCount {threadCount} done")
