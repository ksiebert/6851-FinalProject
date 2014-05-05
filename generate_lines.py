##from __future__ import print_function
import random

def create_test_data(size):
  pass

def create_graphs():
  pass

def generate_random_data(file_name, size, min_length, max_length, window):
  f = open(file_name, 'w')
  print >> f, size
  for i in range(size):
    length = (int)(random.random() * (max_length - min_length) + min_length)
    direction = (int)(random.random() > .5)
    x0 = (int)(random.random() * window)
    y0 = (int)(random.random() * window)
    if direction:
      y1 = y0 + length
      x1 = x0
    else:
      y1 = y0
      x1 = x0 + length
    print >> f, "(" + str(x0) + "," + str(y0) + "),(" + str(x1) + "," + str(y1) + ")" + "(" + str(direction) + ")"

for i in range(10):
  generate_random_data('data/input/test_data' + str(i), 1000, 5, 100, 1000)
