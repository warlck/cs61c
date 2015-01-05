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

def test_distance_map(test_cases = None):
    """
    The value of test_cases should be an array of tests. Each test is
    represented as a dictionary with the following values:

    (Options passed directly to the program)
        left: -l
        right: -r
        width: -w
        height: -h
        threshold: -t
        output: -o

    (Options used by the checker -- These are both file paths)
        actual: The actual bytes produced by the program
        expected: The expected bytes to look for in the program output
    """
    check_command = './depth_map'
    input_folder = 'test/images/'
    output_folder = 'test/output/'
    expected_folder = 'test/expected/'

    if not os.stat(output_folder):
        os.mkdir(output_folder)

    if test_cases is None:
        test_cases = []
        test_cases.append({
            'left': input_folder+'quilt1-left.bmp',
            'right': input_folder+'quilt1-right.bmp',
            'width': 0,
            'height': 0,
            'threshold': 1,
            'output': output_folder+'quilt1-output.bmp',
            'actual': output_folder+'quilt1-output.txt',
            'expected': expected_folder+'quilt1-expected.txt',
            })
        test_cases.append({
            'left': input_folder+'quilt2-left.bmp',
            'right': input_folder+'quilt2-right.bmp',
            'width': 0,
            'height': 0,
            'threshold': 1,
            'output': output_folder+'quilt2-output.bmp',
            'actual': output_folder+'quilt2-output.txt',
            'expected': expected_folder+'quilt2-expected.txt',
            })
        test_cases.append({
            'left': input_folder+'quilt3-left.bmp',
            'right': input_folder+'quilt3-right.bmp',
            'width': 0,
            'height': 0,
            'threshold': 1,
            'output': output_folder+'quilt3-output.bmp',
            'actual': output_folder+'quilt3-output.txt',
            'expected': expected_folder+'quilt3-expected.txt',
            })
        test_cases.append({
            'left': input_folder+'quilt4-left.bmp',
            'right': input_folder+'quilt4-right.bmp',
            'width': 0,
            'height': 0,
            'threshold': 0,
            'output': output_folder+'quilt4-output.bmp',
            'actual': output_folder+'quilt4-output.txt',
            'expected': expected_folder+'quilt4-expected.txt',
            })
        test_cases.append({
            'left': input_folder+'cal1-left.bmp',
            'right': input_folder+'cal1-right.bmp',
            'width': 0,
            'height': 0,
            'threshold': 3,
            'output': output_folder+'cal1-output.bmp',
            'actual': output_folder+'cal1-output.txt',
            'expected': expected_folder+'cal1-expected.txt',
            })
        test_cases.append({
            'left': input_folder+'cal2-left.bmp',
            'right': input_folder+'cal2-right.bmp',
            'width': 0,
            'height': 0,
            'threshold': 4,
            'output': output_folder+'cal2-output.bmp',
            'actual': output_folder+'cal2-output.txt',
            'expected': expected_folder+'cal2-expected.txt',
            })
        test_cases.append({
            'left': input_folder+'real1-left.bmp',
            'right': input_folder+'real1-right.bmp',
            'width': 3,
            'height': 3,
            'threshold': 14,
            'output': output_folder+'real1-output.bmp',
            'actual': output_folder+'real1-output.txt',
            'expected': expected_folder+'real1-expected.txt',
            })
        test_cases.append({
            'left': input_folder+'real2-left.bmp',
            'right': input_folder+'real2-right.bmp',
            'width': 3,
            'height': 3,
            'threshold': 14,
            'output': output_folder+'real2-output.bmp',
            'actual': output_folder+'real2-output.txt',
            'expected': expected_folder+'real2-expected.txt',
            })

    any_failed = False

    print "Testing {} ...".format(check_command)

    for test_case in test_cases:
        try:
            command = [check_command]
            command.extend(['-l', test_case['left']])
            command.extend(['-r', test_case['right']])
            command.extend(['-h', str(test_case['height'])])
            command.extend(['-w', str(test_case['width'])])
            command.extend(['-t', str(test_case['threshold'])])
            command.extend(['-o', test_case['output']])
            command.extend(['-v'])

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

            expected = []
            for line in expected_lines.split('\n'):
                if not line:
                    continue
                match = re.search(r'^(([0-9a-f][0-9a-f] )+)$', line)
                if match is None:
                    raise TestFailedError(
                            'Unrecognized text in output:\n{}'.format(lines))
                matched_bytes = match.group(1).split()
                expected.extend(map(lambda byte: int(byte, 16), matched_bytes))

            lines_file = open(test_case['actual'], 'w')
            lines_file.write(lines)
            lines_file.close()
            for line in lines.split('\n'):
                if not line:
                    continue
                match = re.search(r'^(([0-9a-f][0-9a-f] )+)$', line)
                if match is None:
                    raise TestFailedError(
                            'Unrecognized text in output:\n{}'.format(lines))
                matched_bytes = match.group(1).split()
                for matched_byte in matched_bytes:
                    if not expected or expected[0] != int(matched_byte, 16):
                        raise TestFailedError(
                                'Wrong output. Check {} and {}'.format(
                                test_case['actual'], test_case['expected']))
                    expected.pop(0)
            if expected:
                raise TestFailedError('Wrong output. Check {} and {}'.format(
                    test_case['actual'], test_case['expected']))
        except TestFailedError as err:
            print err.message
            any_failed = True
        except OSError as err:
            print "Error trying to execute program (did you compile?)"
            any_failed = True

    return not any_failed

def test_quadtree(test_cases = None):
    """
    The value of test_cases should be an array of tests.

    (Options used by the checker -- These are both file paths)
        actual: The actual bytes produced by the program
        expected: The expected bytes to look for in the program output
    """
    check_command = './quadtree'
    input_folder = 'test/images/'
    output_folder = 'test/output/'
    expected_folder = 'test/expected/'

    if not os.stat(output_folder):
        os.mkdir(output_folder)

    if test_cases is None:
        test_cases = []
        test_cases.append({
            'input': input_folder+'tiny-input.bmp',
            'actual': output_folder+'tiny-output.txt',
            'expected': expected_folder+'tiny-expected.txt',
            })
        test_cases.append({
            'input': input_folder+'small-input.bmp',
            'actual': output_folder+'small-output.txt',
            'expected': expected_folder+'small-expected.txt',
            })
        test_cases.append({
            'input': input_folder+'empty-input.bmp',
            'actual': output_folder+'empty-output.txt',
            'expected': expected_folder+'empty-expected.txt',
            })
    any_failed = False

    print "Testing {} ...".format(check_command)

    for test_case in test_cases:
        try:
            command = [check_command]
            command.extend([test_case['input']])

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
    distance_map_result = test_distance_map()
    quadtree_result = test_quadtree()
    if distance_map_result and quadtree_result:
        print "All OK"
        sys.exit(0)
    else:
        print "Some tests FAILED"
        sys.exit(1)

