import subprocess
from timing_helper import parse_timing_output

def run_sort_program(program: str,
                     bitsToTest: [int],
                     nToTest: [int],
                     maxSeconds: float,
                     customEnv = None) -> dict:
    timings = {}
    for bits in bitsToTest:
        for count_n in nToTest:
            try:
                print(f"Run test for {bits} bits and n={count_n}")
                result = subprocess.run(
                    [program, str(count_n), str(bits)],
                    capture_output=True,
                    timeout=500,
                    env=customEnv
                )
                print(result.stdout.decode())
                if result.returncode != 0:
                    print(f"Fehler (exit {result.returncode}):")
                    print(result.stderr.decode())
                    continue
                result = parse_timing_output(result.stdout.decode())
                timings[(bits, count_n)] = result
                print(f"{bits},{count_n} done in {result[len(result) - 1][1]:.2f}s")
                if result[len(result) - 1][1] > maxSeconds:
                    print(f"greater 10s -> next bits")
                    break
            except subprocess.TimeoutExpired as e:
                print(f"Timeout -> next bits")
                break
    return timings

if __name__ == "__main__":
    print("This is the time_sort_program.py file and is not meant to run by itself.")
