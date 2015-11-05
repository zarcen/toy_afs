#!/usr/bin/python
import sys
import math
import numpy as np
import matplotlib.pyplot as plt
from os import listdir
from os.path import isfile, join

#===============================
def ReadData(filename):
  with open(filename) as f:
    data = f.read()
  data = data.split('\n')
  return data 

def ReadList3(data):
  n = []
  r1 = []
  r2 = []
  for line in data:
    if line.strip(' '):
      elements = line.split(',')
      n.append(int(elements[0]))
      r1.append(float(elements[1]))
      r2.append(float(elements[2]))
  return n, r1, r2

def ReadList1(data):
  n = []
  w = []
  r1 = []
  r2 = []
  for line in data:
    if line.strip(' '):
      elements = line.split(',')
      n.append(int(elements[0]))
      w.append(float(elements[1]))
      r1.append(float(elements[2]))
      r2.append(float(elements[3]))
  return n, w, r1, r2


#==================================
def DrawFig(filename):
  #average of log time
  data0 = ReadData("norele_single_rw.csv")
  data1 = ReadData("single_rw.csv")
   
  n0, w0, r01, r02 = ReadList1(data0)
  n1, w1, r11, r12 = ReadList1(data1)

  fig = plt.figure()
  ax = fig.add_subplot(111)
  fig.suptitle("With/Without Consistency check")
  plt.xlabel('file size (mb)')
  plt.ylabel('time(ms)')
#ax.set_yscale('log')
#plt.xlim([2.6, 8.3])
  ax.plot(n0, w0, 'r-', label="Without check: write")
  ax.plot(n0, r01, 'r--', label="without check: 1st Read")
  ax.plot(n1, w1, 'b-', label="With check: write")
  ax.plot(n1, r11, 'b--', label="With check: 1st Read")
  ax.legend(bbox_to_anchor=(0.02, 0.98), loc=2, borderaxespad=0.)
  plt.savefig('norele.png')
  plt.show()
  return

#==================================
def main():
  DrawFig("single_rw.csv");
  return

# -------------------------------
# standard boilerplate
if __name__ == '__main__':
  main()
 
