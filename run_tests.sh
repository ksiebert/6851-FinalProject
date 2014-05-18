rm data/input/*
rm data/output/*
rm data/output/qt_cache/*
rm data/output/ds_cache/*
rm data/output/graphs/*
rm data/collisions/*

# generate lines
python generate_lines.py
FILES=data/input/*

# produce cachegrind outputs
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
    cat "$c" | grep "summary" > data/output/qt_cache/$(basename $f)
  done
  echo "Done with qtree cachegrind \n"
  rm dist_sweep
  rm cachegrind.out*
  g++ -o dist_sweep distribution_sweep.cpp
  valgrind --tool=cachegrind ./dist_sweep "$f" | grep "D   refs\|D1  misses"
  CGOUT=cachegrind*
  for c in $CGOUT
  do
    NCGOUT="cache"
    cat "$c" | grep "summary" > data/output/ds_cache/$(basename $f)
  done
  echo "Done with dist_sweep cachegrind \n"
done

# Now run for time
rm qtree
g++ -o qtree quad_tree.cpp
for f in $FILES
do
  ./qtree $f
  echo "Finished qtree "
done
rm dist_sweep
g++ -o dist_sweep distribution_sweep.cpp
for f in $FILES
do
  ./dist_sweep $f
done

python create_graphs.py
rm qtree
rm dist_sweep
rm cachegrind*
