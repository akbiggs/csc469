def read_results_lines(lines):
    results_lines = []
    reading = False
    for line in lines:
        if reading and line.startswith("-"):
            return results_lines

        if line.startswith("Function"):
            reading = True

        if reading:
            results_lines.append(line)

    return results_lines

if __name__ == "__main__":
    with open("stream_out") as f:
        lines = f.readlines()
        print(read_results_lines(lines))
