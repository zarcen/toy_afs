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
      r1.append(float(elements[1]))
      r2.append(float(elements[2]))

  fig = plt.figure()
  ax = fig.add_subplot(111)
  fig.suptitle("Single client: 1st Write,  2nd Write")
  plt.xlabel('file size (mb)')
  plt.ylabel('time(sec)')
  ax.plot(n, r1, 'b^', label="1st write")
  ax.plot(n, r2, 'g^', label="2nd write")
  ax.legend(bbox_to_anchor=(0.1, 0.9), loc=2, borderaxespad=0.)
  plt.savefig('write_twice.png')
  plt.show()
  return

#==================================
def main():
  DrawFig("write_twice.csv");
  return

# -------------------------------
# standard boilerplate
if __name__ == '__main__':
  main()
 
