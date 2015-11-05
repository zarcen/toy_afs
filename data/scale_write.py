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
  for line in data:
    if line.strip(' '):
      elements = line.split(',')
      n.append(int(elements[0]))
      r1.append(float(elements[1]))
  return n, r1

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
  data0 = ReadData("single_rw.csv")
  data1 = ReadData("three_wronly_n3.csv")
  data2 = ReadData("three_wronly_n2.csv")
  data3 = ReadData("three_wronly_n4.csv")
   
  n0, w0, r01, r02 = ReadList1(data0)
  n1, w1 = ReadList3(data1)
  n2, w2 = ReadList3(data2)
  n3, w3 = ReadList3(data3)

  fig = plt.figure()
  ax = fig.add_subplot(111)
  fig.suptitle("Scale [1st Write]: Single client  v.s. Three clients")
  plt.xlabel('file size (mb)')
  plt.ylabel('time(sec)')
#  ax.set_yscale('log')
#plt.xlim([2.6, 8.3])
  ax.plot(n0, w0, 'r-', label="Single Client")
  ax.plot(n1, w1, 'b--', label="1st in 3 Clients")
  ax.plot(n2, w2, 'g--', label="2nd in 3 Clients")
  ax.plot(n3, w3, 'c--', label="3rd in 3 Clients")
  ax.legend(bbox_to_anchor=(0.05, 0.95), loc=2, borderaxespad=0.)
  plt.savefig('scale_write.png')
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
 
