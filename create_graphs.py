import matplotlib.pyplot as plt
from os import listdir
from os.path import isfile, join
def create_plots():
  qt = []
  qt_input_size = []
  qt_collision_size = []
  qt_runtime = []
  qt_cache_misses = []
  qt_wr_refs = []
  qt_rd_refs= []
  qt_wr_cm_d1 = []
  qt_rd_cm_d1= []
  qt_wr_cm_ll = []
  qt_rd_cm_ll= []

  lfs = []
  lfs_input_size = []
  lfs_collision_size = []
  lfs_runtime = []
  lfs_cache_misses = []


  mypath = "data/output/"
  onlyfiles = [ join(mypath, f) for f in listdir(mypath) if isfile(join(mypath,f)) ]
  for i in onlyfiles:
    with open(i, 'r') as f:
      read_data = f.readline()
      if read_data.startswith("qt"):
        qt.append(read_data)
      else:
        lfs.append(read_data)
  for i in qt:
    qt_data = i.split(',')
    qt_input_size.append(int(qt_data[1]))
    qt_collision_size.append(int(qt_data[2]))
    qt_runtime.append(float(qt_data[3]))
    qt_wr_refs.append(int(qt_data[4]))
    qt_rd_refs.append(int(qt_data[5]))
    qt_wr_cm_d1.append(int(qt_data[6]))
    qt_rd_cm_d1.append(int(qt_data[7]))
    qt_wr_cm_ll.append(int(qt_data[8]))
    qt_rd_cm_ll.append(int(qt_data[9]))

  # Plot runtime vs input size
  plt.scatter(qt_input_size, qt_runtime, c='red', s=50)
  plt.show()

  # Plot runtime vs collision size
  plt.scatter(qt_collision_size, qt_runtime, c='red', s=50)
  plt.show()

  # Plot cache misses (writes) vs refs
  plt.scatter(qt_wr_refs, qt_wr_cm_d1)
  plt.show()

  # Plot cache misses (reads) vs refs
  plt.scatter(qt_rd_refs, qt_rd_cm_d1)
  plt.show()

create_plots()

