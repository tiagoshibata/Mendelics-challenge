#!/bin/env python3
import sys

for line in sys.stdin:
    if line[0] == '@':
        # Print headers as-is
        print(line, end='')
        continue
    fields = line.split('\t', maxsplit=5)
    if int(fields[4]) >= 20:  # in (0, 3, 8, 23, 24, 40, 42):
        print(line, end='')
