# Script to read formatted cache output and generate csv files

from os import listdir
from os.path import isfile, join
from itertools import islice
import csv
import re

output_dir = r'C:\Users\Conrad\Documents\GitHub\ECEN4593Cache\all_config_output'
mem_bwidth_dir = r'C:\Users\Conrad\Documents\GitHub\ECEN4593Cache\mem_bwidth_output'

output_files = [f for f in listdir(output_dir) if isfile(join(output_dir,f))]
mem_bwidth_files = [f for f in listdir(mem_bwidth_dir) if isfile(join(mem_bwidth_dir,f))]

# Read the first N lines from each .dat file, match regular expressions,
# and generate a csv file
N = 55 # ignore everything after line 55

# Create a lookup table for line number, start index, and end index
LUT = {5: [(18,22),(32,33),(48,51)],
       6: [(18,22),(32,33),(48,51)],
       7: [(20,25),(35,36),(52,54)],
       8: [(24,26),(41,43),(58,60)],
       10: [(17,29),(45,56)],
       11: [(15,27)],
       12: [(14,24),(39,49)],
       15: [(13,24)],
       16: [(13,24)],
       17: [(13,24)],
       18: [(13,24)],
       21: [(13,24)],
       22: [(13,24)],
       23: [(13,24)],
       24: [(13,24)],
       27: [(11,15),(25,29),(39,43)],
       28: [(22,33),(41,45)], # don't know ideal exec. time width
       29: [(34,45),(53,57)], # don't know idea mis-aligned exec. time width
       # 32 don't know width of miss count
       32: [(16,27),(42,49)],
       # 33 don't know width of total requests
       33: [(21, 32)],
       # Skipping to line 35...don't know any widths
       35: [(15, 21),(40,41),(55,62)],
       # 36 don't know width of flush kickouts
       36: [(21,22)],
       # 39 don't know width of miss count
       39: [(16,27),(42,49)],
       # 40 don't know width of total requests
       40: [(21, 32)],
       # Skipping to line 42...don't know any widths
       42: [(15, 21),(40,41),(55,62)],
       # 43 don't know width of flush kickouts
       43: [(21,22)],
       # 46 don't know width of miss count
       46: [(16,27),(42,49)],
       # 47 don't know width of total requests
       47: [(21, 32)],
       # Skipping to line 35...don't know any widths
       49: [(15, 21),(40,41),(55,62)],
       # 50 don't know width of flush kickouts
       50: [(21,22)],
       # 54 don't know widths
       54: [(12,17),(34,39)]
       }

normal_lines = [5,6,7,8,10,11,12]
weird_lines = [15,16,17,18,21,22,23,24]
normal_lines2 = [32,33,35,36,39,40,42,43,46,47,49,50]
# Special lines (with floats)
special = [27, 28, 29]
# Do something special for 52 and 53 (memory system costs)
cost_lines = [52, 53]

with open('cache_data_everything.csv', 'wt') as csvfile:
    writer = csv.writer(csvfile, delimiter=',', lineterminator='\n')

    writer.writerow(('Filename,Dcache size,Ways,Block size,Icache size,' +
                     'Ways,Block size,L2-cache size,Ways,Block size,' +
                     'Mem ready,Mem chunksize,Mem chunktime,Execute time,' +
                     'Total refs,Flush time,Inst refs,Data refs,Reads,Writes,' +
                     'Inst.,Total,Read cycles,Write cycles,Inst. cycles,' +
                     'Total cycles,L1i hit count,L1i miss count,' +
                     'Total requests,L1i kickouts,L1i dirty kickouts,' +
                     'L1i transfers,L1i flush kickouts,L1d hit count,' +
                     'L1d miss count,L1d total requests,L1d kickouts,' +
                     'L1d dirty kickouts,L1d transfers,L1d flush kickouts,' +
                     'L2 hit count,L2 miss count,L2 total requests,' +
                     'L2 kickouts,L2 dirty kickouts,L2 transfers,' +
                     'L2 flush kickouts,L1i cost,L1d cost,L1 total cost,' +
                     'L2 cost,Mem cost,Total cost,Flushes,Invalidates').split(','))

    for file in output_files:
        # Slice off everything after line 55
        with open(output_dir + '\\' + file) as f:
            head = list(islice(f, N))

        temp = [file]
        
        # Iterate through each normal line
        for line in normal_lines:
            for number in re.findall(r'\d+', head[line]):
                temp.append(number)

        # Iterate through weird lines
        for line in weird_lines:
            temp.append(int(head[line][LUT[line][0][0]:LUT[line][0][1]]))

        # Deal with special lines
        for line in special:
            l = []
            fixed_line = re.sub('[;]', ' ', head[line])
            for t in fixed_line.split():
                try:
                    l.append(float(t))
                except ValueError:
                    pass

        # Iterate through each normal line
        for line in normal_lines2:
            for number in re.findall(r'\d+', head[line]):
                temp.append(number)
        
        # Deal with cost lines
        for line in cost_lines:
            fixed_line = re.sub('[$]', '', head[line])
            fixed_line = re.sub('[)]', '', fixed_line)
            num_list = re.findall(r'\d+', fixed_line)
            del num_list[0]
            #print(num_list)
            for number in num_list:
                temp.append(number)

        # Deal with last line
        line = 54
        for number in re.findall(r'\d+', head[line]):
            temp.append(number)

        del temp[6]
        # Write elements of temp as csv row
        writer.writerow(temp)

    for file in mem_bwidth_files:
        # Slice off everything after line 55
        with open(mem_bwidth_dir + '\\' + file) as f:
            head = list(islice(f, N))

        temp = [file]
        
        # Iterate through each normal line
        for line in normal_lines:
            for number in re.findall(r'\d+', head[line]):
                temp.append(number)

        # Iterate through weird lines
        for line in weird_lines:
            temp.append(int(head[line][LUT[line][0][0]:LUT[line][0][1]]))

        # Deal with special lines
        for line in special:
            l = []
            fixed_line = re.sub('[;]', ' ', head[line])
            for t in fixed_line.split():
                try:
                    l.append(float(t))
                except ValueError:
                    pass

        # Iterate through each normal line
        for line in normal_lines2:
            for number in re.findall(r'\d+', head[line]):
                temp.append(number)
        
        # Deal with cost lines
        for line in cost_lines:
            fixed_line = re.sub('[$]', '', head[line])
            fixed_line = re.sub('[)]', '', fixed_line)
            num_list = re.findall(r'\d+', fixed_line)
            del num_list[0]
            #print(num_list)
            for number in num_list:
                temp.append(number)

        # Deal with last line
        line = 54
        for number in re.findall(r'\d+', head[line]):
            temp.append(number)

        del temp[6]
        # Write elements of temp as csv row
        writer.writerow(temp)
