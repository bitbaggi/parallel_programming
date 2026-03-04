import csv
import re

def parse_timing_output(text: str) -> list[(int, (float, float), (float, float), (float, float))]:
    runs = []

    for line in text.strip().splitlines():
        run_match = re.match(r"Detailed Timing of Run (\d+):", line)
        avg_match = re.match(r"Average Time of the Runs:", line)
        total_match = re.match(r"Average time taken: ([\d.]+) seconds", line)

        if not run_match and not avg_match and not total_match:
            continue

        run_id = int(run_match.group(1)) if run_match else None

        pairs = re.findall(r"([\d.]+)\s*\(([\d.]+)\s*%\)", line)
        parts = [(float(time), float(perc)) for time, perc in pairs]

        avg_time = float(total_match.group(1)) if total_match else None
        runs.append((run_id, avg_time, parts))

    return runs


def write_timings_to_file(file_name: str, timings_to_write: dict):
    with open(file_name, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(
            ['bits', 'numberCount', 'run_id', 'part1_time', 'part1_pct', 'part2_time', 'part2_pct', 'part3_time',
             'part3_pct', 'avg_time'])

        for (bits, numberCount), runs in timings_to_write.items():
            for run_id, avg_time, parts in runs:
                row = [bits, numberCount, run_id if run_id is not None else 'avg']
                for t, pct in parts:
                    row += [t, pct]
                while len(row) < 9:
                    row += [None, None]
                row.append(avg_time)
                writer.writerow(row)


if __name__ == "__main__":
    print("This is the timing_parser.py file and is not meant to run by itself.")
