#!/usr/bin/env python

import os
import subprocess
import sys
import re
import resource

class TestFailedError(Exception):
    pass

def start_process(args):
    print "Running: {}".format(' '.join(args))
    process = subprocess.Popen(args,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE)
    return process

def test_lfsr():

    expected_values = [26625, 5185, 27515, 38801, 56379, 52819, 14975, 21116, 38463, 54726, 38049, 26552, 4508, 46916, 37319, 41728, 23224, 26004, 11119, 62850]

    print "Testing lfsr..."

    try:
        process = start_process(['./mars', 'random.s'])
        lines = process.stdout.read()
        process.wait()

        if process.returncode != 0:
            raise TestFailedError('Non-zero return-code')
    except OSError as err:
        print "Error trying to execute program (did you compile?)"
        any_failed = True

    for line in lines.split('\n'):
        if expected_values and str(expected_values[0]) in line:
            print "  Found: {0}".format(expected_values.pop(0))
    if expected_values:
        print "Couldn't find expected values: {0}".format(', '.join(map(str, expected_values)))

    return not expected_values

def test_quad2matrix():

    expected_lines = [
        "  -----0----------1----------2----------3----------4----------5----------6----------7-----",
        "0 0x00000001 0x00000001 0x00000001 0x00000001 0x00000003 0x00000003 0x00000004 0x00000004 ",
        "1 0x00000001 0x00000001 0x00000001 0x00000001 0x00000003 0x00000003 0x00000004 0x00000004 ",
        "2 0x00000001 0x00000001 0x00000001 0x00000001 0x00000005 0x00000005 0x00000009 0x0000000a ",
        "3 0x00000001 0x00000001 0x00000001 0x00000001 0x00000005 0x00000005 0x0000000c 0x0000000b ",
        "4 0x00000002 0x00000002 0x00000002 0x00000002 0x00000006 0x00000006 0x00000007 0x00000007 ",
        "5 0x00000002 0x00000002 0x00000002 0x00000002 0x00000006 0x00000006 0x00000007 0x00000007 ",
        "6 0x00000002 0x00000002 0x00000002 0x00000002 0x00000008 0x00000008 0x0000000d 0x0000000e ",
        "7 0x00000002 0x00000002 0x00000002 0x00000002 0x00000008 0x00000008 0x00000010 0x0000000f ",
    ]

    print "Testing quad2matrix..."

    try:
        process = start_process(['./mars', 'matrix.s'])
        lines = process.stdout.read()
        process.wait()

        if process.returncode != 0:
            raise TestFailedError('Non-zero return-code')
    except OSError as err:
        print "Error trying to execute program (did you compile?)"
        any_failed = True

    for line in lines.split('\n'):
        if expected_lines and str(expected_lines[0]) in line:
            print "  Found: {0}".format(expected_lines.pop(0))
    if expected_lines:
        print "Couldn't find expected values: {0}".format('\n'.join(expected_lines))

    return not expected_lines

def test_autostereogram(test_cases = None):
    """
    The value of test_cases should be an array of tests.

    (Options passed directly to the program)
        input: The input depth_map
        output: An output file
        strip_size: The size of the autostereogram strip

    (Options used by the checker)
        actual: The actual output
        expected: The expected output file
    """
    input_folder = 'test/images/'
    output_folder = 'test/output/'
    expected_folder = 'test/expected/'

    if not os.path.exists(output_folder):
        os.mkdir(output_folder)

    if test_cases is None:
        test_cases = []
        test_cases.append({
            'input': input_folder+'blob1-input.bmp',
            'output': output_folder+'blob1-output.bmp',
            'actual': output_folder+'blob1-output.txt',
            'expected': expected_folder+'blob1-expected.txt',
            'strip_size': 50,
            })
        test_cases.append({
            'input': input_folder+'blob2-input.bmp',
            'output': output_folder+'blob2-output.bmp',
            'actual': output_folder+'blob2-output.txt',
            'expected': expected_folder+'blob2-expected.txt',
            'strip_size': 47,
            })
    any_failed = False

    print "Testing autostereogram ..."

    for test_case in test_cases:
        try:
            command = ['./mars', 'autostereogram.s', 'pa']
            command.append(test_case['input'])
            command.append(test_case['output'])
            command.append(str(test_case['strip_size']))
            command.append('--verbose')

            process = start_process(command)

            lines = process.stdout.read()
            process.wait()
            if process.returncode != 0:
                raise TestFailedError('Non-zero return-code')
            try:
                expected_lines_file = open(test_case['expected'], 'r')
                expected_lines = expected_lines_file.read()
            except IOError:
                raise TestFailedError(
                        "Can't find expected lines file: {}".format(
                            test_case['expected']))

            lines_file = open(test_case['actual'], 'w')
            lines_file.write(lines)
            lines_file.close()

            if lines != expected_lines:
                raise TestFailedError('Wrong output. Check {} and {}'.format(
                    test_case['actual'], test_case['expected']))
        except TestFailedError as err:
            print err.message
            any_failed = True
        except OSError as err:
            print "Error trying to execute program (did you compile?)"
            any_failed = True

    return not any_failed

if __name__ == '__main__':
    lfsr_result = test_lfsr()
    quad2matrix_result = test_quad2matrix()
    autostereogram_result = test_autostereogram()
    if lfsr_result and quad2matrix_result and autostereogram_result:
        print "All OK"
        sys.exit(0)
    else:
        print "Some tests FAILED"
        sys.exit(1)

