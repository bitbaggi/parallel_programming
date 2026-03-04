import subprocess
import re

from dataclasses import dataclass
from typing import Optional


@dataclass
class TimingEntry:
    time: float
    percent: float


@dataclass
class TimingRun:
    run_id: Optional[int]  # None = Average
    parts: list[TimingEntry]


def parse_timing_output(text: str) -> list[TimingRun]:
    runs = []

    for line in text.strip().splitlines():
        # Run oder Average?
        run_match = re.match(r"Detailed Timing of Run (\d+):", line)
        avg_match = re.match(r"Average Time of the Runs:", line)

        if not run_match and not avg_match:
            continue

        run_id = int(run_match.group(1)) if run_match else None

        # Alle (time, percent) Paare extrahieren
        pairs = re.findall(r"([\d.]+)\s*\(([\d.]+)\s*%\)", line)
        parts = [TimingEntry(time=float(t), percent=float(p)) for t, p in pairs]

        runs.append(TimingRun(run_id=run_id, parts=parts))

    return runs


MAX_SECONDS = 10
BIT_COUNTS_TO_TEST = [2, 4, 8, 16]
max_n_by_bits = {}

for bits in BIT_COUNTS_TO_TEST:
    for numberCount in range(10000, 10000000000, 50000000):
        output = subprocess.check_output(["../problem_1", f"{numberCount} {bits}"])
        timings = parse_timing_output(output.decode())
        for run in timings:
            if run.run_id is None:
                print(f"Bits: {bits}, Count: {numberCount}")
                for i, part in enumerate(run.parts):
                    print(f"  Part #{i}, Time: {part.time:.2f}s, Percent: {part.percent:.2f}%")
