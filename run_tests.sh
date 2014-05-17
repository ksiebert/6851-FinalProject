rm data/input/*
rm data/output/*
rm data/output/qt_cache/*
rm data/collisions/*
python generate_lines.py
FILES=data/input/*
for f in $FILES
do
  rm qtree
  rm cachegrind.out*
  g++ -o qtree quad_tree.cpp
  valgrind --tool=cachegrind ./qtree "$f" | grep "D   refs\|D1  misses"
  CGOUT=cachegrind*
  for c in $CGOUT
  do
    NCGOUT="cache"
    cat "$c" | grep "summary" > $f
    cp $f data/output/qt_cache/
  done
  rm dist_sweep
  rm cachegrind.out*
  g++ -o dist_sweep distribution_sweep.cpp
  valgrind --tool=cachegrind ./qtree "$f" | grep "D   refs\|D1  misses"
  CGOUT=cachegrind*
  for c in $CGOUT
  do
    NCGOUT="cache"
    cat "$c" | grep "summary" > $f
    cp $f data/output/ds_cache/
  done
done

rm data/input/*
rm data/output/*
rm data/collisions/*
python generate_lines.py
rm qtree
g++ -o qtree quad_tree.cpp
for f in $FILES
do
  ./qtree $f
done
python migrate_cache_data.py
rm data/output/qt_cache/*
rm qtree
rm cachegrind*
python create_graphs.py
