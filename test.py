
from glob import *
import os.path

for path, dirs, files in os.walk('/home/witut/Desktop/xaninOS'):
  print(path)
  for f in files:
    print(os.path.abspath(f))
