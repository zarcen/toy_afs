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
  return n, r1, r2


#==================================
def DrawFig(filename):
  #average of log time
  data0 = ReadData("consistcheck.csv")
  data1 = ReadData("consistcheck_wichcache.csv")
   
  n0, r01, r02 = ReadList3(data0)
  n1, r11, r12 = ReadList3(data1)

  fig = plt.figure()
  ax = fig.add_subplot(111)

  fig.suptitle("Read: with / without previous crash")
  plt.xlabel('file size (mb)')
  plt.ylabel('time(sec)')
  ax.plot(n0, r01, 'r^', label="1st read")
  ax.plot(n0, r02, 'b^', label="2nd read (Crash after last write)")
  ax.plot(n1, r11, 'g^', label="2nd read (read from cache)")
  
  ax.legend(bbox_to_anchor=(0.1, 0.9), loc=2, borderaxespad=0.)
  plt.savefig('consistency.png')
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
 
