g++ distribution_sweep.cpp -o dist_sweep
g++ quad_tree.cpp -o qtree
FILE=data/input/test_data10
./dist_sweep $FILE 256
./qtree $FILE
rm dist_sweep
rm qtree


