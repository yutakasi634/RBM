# -*- coding: utf-8 -*-

import csv
import sys

args = sys.argv

if args[1].find('.') == -1:
    print('拡張子がありません')

fout = open(args[1][0:args[1].find('.')] + '.csv', 'w')
csvWriter = csv.writer(fout)

char_list = []

for line in open(args[1], 'r'):
    if line[0] == "#":
        continue
    elif line.isspace():
        csvWriter.writerow(char_list)
        char_list = []
    else:
        line = line.replace('\n','')
        line = line.replace(',','')
        char_list.extend(list(line))

csvWriter.writerow(char_list)
char_list = []
fout.close()
