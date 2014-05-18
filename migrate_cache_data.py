from os import listdir
from os.path import isfile, join
def migrate_data():
  qt = []
  qt_wr_refs = []
  qt_rd_refs= []
  qt_wr_cm_d1 = []
  qt_rd_cm_d1= []
  qt_wr_cm_ll = []
  qt_rd_cm_ll= []

  mypath = "data/output/qt_cache/"
  onlyfiles = [ f for f in listdir(mypath) if isfile(join(mypath,f)) ]
  for i in onlyfiles:
    with open(mypath + i, 'r') as f:
      qt_data = f.readline().split(' ')
      newpath = "data/output/qt_" + i
      with open(newpath, 'a') as f2:
        f2.write("," + ",".join(qt_data[4:]))
          
migrate_data()

