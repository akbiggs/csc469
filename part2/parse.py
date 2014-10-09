import os
from collections import OrderedDict
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
    fn_times = OrderedDict()
    for cpu,cpu_results in data.iteritems():
        fn_times[cpu] = [float(result['Min time'][fn_index]) for result in cpu_results]

    return fn_times

def average_attrs(cpu_datalist, attrs, num_fns):
    averages = {}
    for attr in attrs:
        averages[attr] = []
        for i in range(num_fns):
            fn_attr_sum = sum(float(cpu_data[attr][i]) for cpu_data in cpu_datalist)
            averages[attr].append(fn_attr_sum / len(cpu_datalist))
    
    return averages

def print_table(avg_cpu_data, column_names, fn_names):
    # print columns
    for col in column_names:
        print "%14s" % col,
    print ""

    attr_names = [name for name in column_names if name != "Function"]

    for i,fn_name in enumerate(fn_names):
        print "%14s" % fn_name,
        for attr in attr_names:
            print "%14f" % avg_cpu_data[attr][i],
        print ""
    
if __name__ == "__main__":
    data = {}
    fn_names = []
    column_names = []
    
    # Run the test on various CPUs connecting to node 0.
    for cpu in [i for i in range(0, 48)]:
        data[cpu] = [run_stream_on(0, cpu) for i in range(0, 3)]
        print("Completed tests for CPU {0}".format(cpu))
        
        # Fill out the function names array if needed.
        if not len(fn_names):
            fn_names = data[cpu][0]['Function']
            column_names = data[cpu][0].keys()

    print(data)
    # For each function in the function names array, generate a data file.
    for i in range(0, 4):
        fn_name = fn_names[i]
        with open("data{0}.dat".format(fn_name.strip(":")), "w") as f:
            j = 0
            for cpu, avg_times in get_average_fn_times(data, i).iteritems():
                avg = sum(avg_times) / len(avg_times)
                f.write("{0} {1} {2}\n".format(j, cpu, avg))
                j += 1
    
    attrs = [col for col in column_names if col != "Function"]
    cpu_datalist = [datapoint for _,cpu_data in data.iteritems() for datapoint in cpu_data]

    avgs = average_attrs(cpu_datalist, attrs, len(fn_names))
    print_table(avgs, column_names, fn_names)
