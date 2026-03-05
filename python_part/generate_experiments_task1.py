from time_sort_program import run_sort_program
from timing_helper import write_timings_to_file

MAX_SECONDS = 10
STEP_SIZE = 20000000
MAX_NUMBER_COUNT = 10000000000
BIT_COUNTS_TO_TEST = [1, 2, 4, 8, 16]

seq_timings = run_sort_program('../problem_1', BIT_COUNTS_TO_TEST,
                               {n for n in range(50000000, MAX_NUMBER_COUNT, STEP_SIZE)},
                               MAX_SECONDS, customEnv={})

write_timings_to_file('timings_sequential.csv', seq_timings)
