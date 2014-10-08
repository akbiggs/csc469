import os
from subprocess import Popen, PIPE

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
    
def run_stream_on(node_number, cpu_number):
	process = Popen(["numactl", "--membind", str(node_number), "--physcpubind",str(cpu_number), "/u/csc469h/fall/pub/bin/mccalpin-stream"], stdout = PIPE)
	(output, err) = process.communicate()
	exit_code = process.wait()
	
	return parse_data(read_results_lines(output.split("\n")))

def get_average_fn_times(data, fn_index):
	fn_times = {}
	for cpu,cpu_results in data.iteritems():
		fn_times[cpu] = [float(result['Avg time'][fn_index]) for result in cpu_results]
	
	return fn_times

if __name__ == "__main__":
	data = {}
	for cpu in [i * 4 for i in range(0, 2)]:
		data[cpu] = [run_stream_on(0, cpu) for i in range(0, 2)]
		print(cpu)
		
	print(get_average_fn_times(data, 0))
