#!/usr/bin/python3
# Running the tests in this testsuite.

import os
import logging
import re
import time
import subprocess
import json
import argparse

cur_file = os.path.abspath(__file__)
test_dir = os.path.dirname(cur_file)
proj_dir = os.path.dirname(test_dir)

build_sh = os.path.join(test_dir, "run_build.sh")
expected_file = os.path.join(test_dir, "expected.json")
out_log_file = os.path.join(test_dir, "out.log")
trace_log_file = os.path.join(test_dir, "trace.log")
time_file = os.path.join(test_dir, "time.csv")

TRACE_OPTIONS = " trace 1 1 trace 2 1 trace 3 1 "
TPERF_OPTIONS = " trace 4 1"
TPROFILE_OPTIONS = " trace 4 1 "

time_pattern = r'-?([1-9]\d*\.\d*|0\.\d*[1-9]\d*|0?\.0+|0)ms'
prof_pattern = r'([0-9]+)'
prof_trace_pattern = r'.*Spent time = ([0-9\.]+)ms, IPS = ([0-9\.]+), Final ret = ([0-9\.]+)'

def run_cmd(cmd_str: str):
    # logging.info(">> %s", cmd_str)
    with open(out_log_file, "w+") as outf, open(trace_log_file, "w+") as tracef:
        print("CMD: %s" % cmd_str)
        print("Out: %s, Trace: %s" % (out_log_file, trace_log_file))
        s = subprocess.run(cmd_str, stdout=outf, stderr=tracef, shell=True)
        outf.flush()
        ret = s.returncode
        if ret != 0:
            raise Exception("Execute of command '%s' failed." % cmd_str)
        else:
            outf.seek(0, 0)
            return ret


def compare_results(test_id_int: int):
    test_id = str(test_id_int)
    with open(trace_log_file, "r") as outf:
        last_lines= outf.readlines()
        last_line = last_lines[-1]
        logging.info('result: %s' % last_line)
        if expected_res.get(test_id) and last_line != expected_res.get(test_id):
            raise Exception("Comparison failed for case id = %d, res: %s, expect: %s" % (
                test_id_int, last_line, expected_res.get(test_id)))
        expected_res[test_id] = last_line


def run_uvm_with_uwasm_func(test_id: int, test_file: str, init_func: int, init_reg1: int, init_val: int):
    os.chdir(proj_dir)
    logging.info("Running %s ...", os.path.basename(test_file))
    run_cmd_line = "./build-release/uvm %s file %s func %x sreg %d %d " % (
        TRACE_OPTIONS, test_file, init_func, init_reg1, init_val)
    run_cmd(run_cmd_line)
    compare_results(test_id)


def run_c_case(test_id: int, test_c_file: str, init_func: int, init_reg1: int, init_val: int):
    pre, ext = os.path.splitext(test_c_file)
    uwasm_file = pre + ".uwm"
    run_cmd_line = "xvsa -O2 -noxa -noxfa -sw -c %s -o %s" % \
                   (test_c_file, uwasm_file)
    logging.info("Compiling %s ...", os.path.basename(test_c_file))
    run_cmd(run_cmd_line)
    run_uvm_with_uwasm_func(test_id, uwasm_file, init_func, init_reg1, init_val)


def run_uvm_perf(test_id: int, test_file: str, init_func: int, init_reg1: int, init_val: int):
    os.chdir(proj_dir)
    run_cmd_line = "./build-release/uvm %s file %s func %d sreg %d %d " % (
        TPERF_OPTIONS, test_file, init_func, init_reg1, init_val)
    run_cmd(run_cmd_line)
    with open(trace_log_file, "r") as outf:
        last_lines= outf.readlines()
        last_line = last_lines[-1]
        spent_time = re.compile(time_pattern).findall(last_line)[0]
        with open(time_file, "a") as timef:
            timef.write("{},{},{},{},{},{}\n".format(os.path.basename(test_file), init_func, init_reg1, init_val,
                                          time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()), spent_time))

def run_uvm_profile(test_id: int, test_file: str, init_func: int, init_reg1: int, init_val: int):
    os.chdir(proj_dir)
    run_cmd_line = "./build-release/uvm %s file %s func %d sreg %d %d " % (
        TPROFILE_OPTIONS, test_file, init_func, init_reg1, init_val)
    logging.info("%s", run_cmd_line)
    run_cmd(run_cmd_line)

    # Current format:
    # Spent time = XXX.XXXms, IPS = XXX.XX, Final ret = XXXXX
    with open(trace_log_file, "r") as outf:
        last_lines= outf.readlines()
        last_line = last_lines[-1]
        instr_lines = last_lines[0:-1]

        # time_info = re.compile(time_pattern).findall(last_line)[0]
        match_res = re.compile(prof_trace_pattern).match(last_line)
        if len(match_res.groups()) < 1:
            raise Exception("Cannot process uvm trace info, please check its format.")
        time_spent = float(match_res.groups()[0])
        ips = float(match_res.groups()[1])
        instr_count = int(ips * time_spent / 1000.0)
        instr_count_file = "instr_count.log"
        with open(instr_count_file, "a") as logf:
            logf.write("%d, %d, %f\n" % (init_val, instr_count, time_spent))
        logging.info("%d, %d, %f", init_val, instr_count, time_spent)


def run_uvm_fib(i: int):
    fib3_uwasm = os.path.join(test_dir, "as", "cgfib", "cgfib.uwm")
    logging.info("WASM   Fib(%d) Spent time = %d.00000ms", i, wasm_time[i])

    run_uvm_perf(5, fib3_uwasm, 1, 1, i)
    with open(trace_log_file, "r") as outf:
        last_line = outf.readlines()[-1]
        logging.info("UWASM  Fib(%d) %s", i, last_line.replace('\n', ''))

def run_uvm_fib_profile(i: int):
    fib3_uwasm = os.path.join(test_dir, "as", "cgfib", "cgfib.o2.uwm")
    run_uvm_profile(5, fib3_uwasm, 1, 1, i)


def run_tests(perf: bool, withxvsa: bool, extra_perf: bool):
    os.chdir(proj_dir)
    run_cmd("bash " + build_sh)

#    fib3_uwasm = os.path.join(test_dir, "as", "fib3", "fib3.uwasm")
#    run_uvm_with_uwasm_func(1, fib3_uwasm, 0, 2, 16)
#    run_uvm_with_uwasm_func(2, fib3_uwasm, 0, 2, 20)
#    run_uvm_with_uwasm_func(3, fib3_uwasm, 0, 2, 10)

    if withxvsa:
        # Run the test by compiling first
        fib_c_case = os.path.join(test_dir, "as", "cgfib", "cgfib.c")
        run_c_case(6, fib_c_case, 1, 1, 16)
        malloc_c_case = os.path.join(test_dir, "sanity", "tcall-malloc.c")
        run_c_case(11, malloc_c_case, 0, 0, 0)
        putchar_c_case = os.path.join(test_dir, "sanity", "tcall-putchar.c")
        run_c_case(12, putchar_c_case, 0, 0, 0)

    fib3_uwasm = os.path.join(test_dir, "as", "cgfib", "cgfib.precomp.uwm")
    # run_uvm_with_uwasm_func(4, fib3_uwasm, 1, 1, 20)

    fib3_uwasm = os.path.join(test_dir, "as", "cgfib", "cgfib.precomp.uwm")
    # run_uvm_with_uwasm_func(4, fib3_uwasm, 1, 1, 16)

    # Run a performance testing
    run_uvm_fib(33)
    if perf:
        for i in range(33, 45):
            run_uvm_fib(i)

    if extra_perf:
        for i in range(33, 45):
            run_uvm_fib_profile(i)

    # fib_c_case = os.path.join(test_dir, "as", "sanity", "t-conv.cxx")
    # run_c_case(7, fib_c_case, 1, 0, 0)

    # fib_c_case = os.path.join(test_dir, "sanity", "t-div.cxx")
    # run_c_case(8, fib_c_case, 1, 0, 0)

    # fib_c_case = os.path.join(test_dir, "sanity", "t-mul.cxx")
    # run_c_case(9, fib_c_case, 1, 0, 0)

    # fib_c_case = os.path.join(test_dir, "sanity", "t-sub.cxx")
    # run_c_case(10, fib_c_case, 1, 0, 0)

    # run uvm_api_test
    os.chdir(os.path.join(proj_dir, "build-release"))
    run_uvm_all_tests = "./all_tests"
    run_cmd(run_uvm_all_tests)
    os.chdir(proj_dir)

    json.dump(expected_res, open(expected_file, "w"))


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    parser = argparse.ArgumentParser(description="Sanity / Performance testing")
    parser.add_argument('-p', '--perf', action='store_true', default=False,
                        help='Run performance tests besides sanity tests (default: %(default)s)')
    parser.add_argument('--extra_perf', action='store_true', default=False,
                        help='Run extra performance tests (default: %(default)s)')
    parser.add_argument('-n', '--noxvsa', action='store_true', default=False,
                        help='Run without xvsa (default: %(default)s)')

    args = parser.parse_args()

    # fib(0~40)
    js_time = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 2, 4, 5, 8, 12, 18, 29, 45,
               73, 121, 190, 308, 499, 830, 1342, 2130, 3438, 5618, 9124, 14840, 27968, 46096, 74889, 124488]
    wasm_time = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 4, 6, 9, 12, 19,
                 31, 49, 80, 127, 200, 331, 536, 915, 1563, 2387, 4289, 7778, 10483, 0, 0, 0, 72376]

    expected_res = {}
    if os.path.exists(expected_file):
        expected_res = json.load(open(expected_file, "r"))

    run_tests(args.perf, not(args.noxvsa), args.extra_perf)
