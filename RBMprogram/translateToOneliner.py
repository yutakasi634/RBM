# -*- coding: utf-8 -*-

import csv
import sys

args = sys.argv

if args[1].find('.') == -1:
    print('拡張子がありません')

fin = open(args[1],'r')
reader = csv.reader(fin)
fout = open(args[1][0:args[1].find('.')] + '.csv', 'w')
csvWriter = csv.writer(fout)

char_list = []

for line in reader:
    if line == []:
        print(char_list)
        csvWriter.writerow(char_list)
        char_list = []
    elif line[0] == "#":
        continue
    else:
        if line[-1] == '':
            del line[-1]
        for i in range(0,len(line)):
            char_list.append(float(line[i]))
if char_list != []:
    csvWriter.writerow(char_list)

fin.close()
fout.close()
