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

  ds = []
  ds_input_size = []
  ds_collision_size = []
  ds_runtime = []
  ds_cache_misses = []
  ds_wr_refs = []
  ds_rd_refs= []
  ds_wr_cm_d1 = []
  ds_rd_cm_d1= []
  ds_wr_cm_ll = []
  ds_rd_cm_ll= []


  mypath = "data/output/"
  onlyfiles = [ join(mypath, f) for f in listdir(mypath) if isfile(join(mypath,f)) ]
  for i in onlyfiles:
    with open(i, 'r') as f:
      read_data = f.readline()
      if read_data.startswith("qt"):
        qt.append(read_data)
      elif read_data.startswith("ds"):
        ds.append(read_data)
      
  for i in qt:
    qt_data = i.split(',')
    qt_input_size.append(int(qt_data[1]))
    qt_collision_size.append(int(qt_data[2]))
    qt_runtime.append(float(qt_data[3]))
    
  for f in listdir("data/output/qt_cache/"):
    with open(join("data/output/qt_cache/",f), 'r') as open_file:
      data = open_file.readline()
      qt_data = data.split(' ')
      qt_rd_refs.append(int(qt_data[4]))
      qt_rd_cm_d1.append(int(qt_data[5]))
      qt_rd_cm_ll.append(int(qt_data[6]))
      qt_wr_refs.append(int(qt_data[7]))
      qt_wr_cm_d1.append(int(qt_data[8]))
      qt_wr_cm_ll.append(int(qt_data[9]))

  for i in ds:
    ds_data = i.split(',')
    ds_input_size.append(int(ds_data[1]))
    ds_collision_size.append(int(ds_data[2]))
    ds_runtime.append(float(ds_data[3]))

  for f in listdir("data/output/ds_cache/"):
    with open(join("data/output/ds_cache/",f), 'r') as open_file:
      data = open_file.readline()
      ds_data = data.split(' ')
      ds_rd_refs.append(int(ds_data[4]))
      ds_rd_cm_d1.append(int(ds_data[5]))
      ds_rd_cm_ll.append(int(ds_data[6]))
      ds_wr_refs.append(int(ds_data[7]))
      ds_wr_cm_d1.append(int(ds_data[8]))
      ds_wr_cm_ll.append(int(ds_data[9]))


  output_folder = "data/output/graphs/"
  # Plot runtime vs input size
  plt.scatter(qt_input_size, qt_runtime, c='red', s=50)
  plt.scatter(ds_input_size, ds_runtime, c='blue', s=50)
  plt.xlabel('Input Size')
  plt.ylabel('Runtime (s)')
  plt.savefig(join(output_folder,'input_runtime.png'), bbox_inches='tight')
  plt.close()

  # Plot runtime vs collision size
  plt.scatter(qt_collision_size, qt_runtime, c='red', s=50)
  plt.scatter(qt_collision_size, ds_runtime, c='blue', s=50)
  plt.xlabel('Number of Collisions')
  plt.ylabel('Runtime (s)')
  plt.savefig(join(output_folder,'coll_runtime.png'), bbox_inches='tight')
  plt.close()

  # Plot cache misses (writes) vs refs (qt)
  plt.scatter(qt_wr_refs, qt_wr_cm_d1, c='red')
  plt.xlabel('D1 Cache Write References')
  plt.ylabel('D1 Cache Write Misses')
  plt.savefig(join(output_folder,'refs_write_qt.png'), bbox_inches='tight')
  plt.close()

  # Plot cache misses (writes) vs refs (ds)
  plt.scatter(ds_wr_refs, ds_wr_cm_d1, c='blue')
  plt.xlabel('D1 Cache Write References')
  plt.ylabel('D1 Cache Write Misses')
  plt.savefig(join(output_folder,'refs_writes_ds.png'), bbox_inches='tight')
  plt.close()

  # Plot cache misses (reads) vs refs (qt)
  plt.scatter(qt_rd_refs, qt_rd_cm_d1, c='red')
  plt.xlabel('D1 Cache Read References')
  plt.ylabel('D1 Cache Read Misses')
  plt.savefig(join(output_folder,'refs_reads_qt.png'), bbox_inches='tight')
  plt.close()

  # Plot cache misses (reads) vs refs (ds)
  plt.scatter(ds_rd_refs, ds_rd_cm_d1, c='blue')
  plt.xlabel('D1 Cache Write References')
  plt.ylabel('D1 Cache Write Misses')
  plt.savefig(join(output_folder,'refs_reads_ds.png'), bbox_inches='tight')
  plt.close()

  # # Plot cache misses (reads) vs input size
  plt.scatter(qt_input_size, qt_rd_cm_d1, c='red')
  plt.scatter(ds_input_size, ds_rd_cm_d1, c='blue')
  plt.xlabel('Input Size')
  plt.ylabel('D1 Cache Read Misses')
  plt.savefig(join(output_folder,'input_reads.png'), bbox_inches='tight')
  plt.close()
  
create_plots()

