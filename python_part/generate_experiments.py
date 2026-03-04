import csv
import subprocess
from timing_parser import parse_timing_output

MAX_SECONDS = 10
STEP_SIZE = 20000000
MAX_NUMBER_COUNT = 1000000000
BIT_COUNTS_TO_TEST = [1, 2, 4, 8, 16]

timings = {}
for bits in BIT_COUNTS_TO_TEST:
    for numberCount in range(50000000, MAX_NUMBER_COUNT, STEP_SIZE):

        try:
            output = subprocess.check_output(["../problem_1", f"{numberCount} {bits}"], timeout=300)
            result = parse_timing_output(output.decode())
            print(f"{bits},{numberCount} done in {result[len(result)-1][1]:.2f}s")
            if result[len(result)-1][1] > MAX_SECONDS:
                print(f"greater 10s -> next bits")
                break
            timings[(bits, numberCount)] = result
        except subprocess.TimeoutExpired as e:
            print(f"Timeout -> next bits")
            break

with open('timings.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(
        ['bits', 'numberCount', 'run_id', 'part1_time', 'part1_pct', 'part2_time', 'part2_pct', 'part3_time',
         'part3_pct', 'avg_time'])

    for (bits, numberCount), runs in timings.items():
        for run_id, avg_time, parts in runs:
            row = [bits, numberCount, run_id if run_id is not None else 'avg']
            for t, pct in parts:
                row += [t, pct]
            while len(row) < 9:
                row += [None, None]
            row.append(avg_time)
            writer.writerow(row)