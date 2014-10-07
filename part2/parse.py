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

def parse_row(row):
    return [x.strip() for x in row.split("  ") if x != ""]

def parse_data(results_lines):
    header_line, rows = results_lines[0], results_lines[1:]
    columns = [x.strip() for x in header_line.split("  ") if x != ""]
    parsed_rows = [parse_row(row) for row in rows]

    column_data = {}
    for i in range(0, len(columns)):
        column_data[columns[i]] = [row[i] for row in parsed_rows]

    return column_data

if __name__ == "__main__":
    with open("stream_out") as f:
        data = parse_data(read_results_lines(f.readlines()))
        print(data)
