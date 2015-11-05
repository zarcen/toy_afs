#!/usr/bin/python
import sys
import math
import numpy as np
import matplotlib.pyplot as plt
from os import listdir
from os.path import isfile, join

#==================================
def DrawFig(filename):
  x = []
  avg = []
  sdv = []

  #average of log time
  with open(filename) as f:
    data = f.read()
  data = data.split('\n')
    
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

  fig = plt.figure()
  ax = fig.add_subplot(111)
  fig.suptitle("Single client:  Write,  1st Read,  2nd Read")
  plt.xlabel('file size (mb)')
  plt.ylabel('time(sec)')
#  ax.set_yscale('log')
#plt.xlim([2.6, 8.3])
  ax.plot(n, w, 'ro', label="write")
  ax.plot(n, r1, 'b^', label="1st read")
  ax.plot(n, r2, 'g^', label="2nd read")
  ax.legend(bbox_to_anchor=(0.2, 0.8), loc=2, borderaxespad=0.)
  plt.savefig('single.png')
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
 
