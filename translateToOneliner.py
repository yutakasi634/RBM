# -*- coding: utf-8 -*-

import csv

fout = open('data.csv', 'w')
csvWriter = csv.writer(fout)

char_list = []

for line in open('data.dat', 'r'):
    if line[0] == "#":
        continue
    elif line.isspace():
        csvWriter.writerow(char_list)
        char_list = []
    else:
        line = line.replace('\n','')
        char_list.extend(list(line))

csvWriter.writerow(char_list)
char_list = []
fout.close()
