rm data/output/*
rm data/input/*
python generate_lines.py
g++ distribution_sweep.cpp -o dist_sweep
g++ quad_tree.cpp -o qtree
FILE=data/input/test_data1
./dist_sweep $FILE
./qtree $FILE
FILE=data/input/test_data10
./dist_sweep $FILE
./qtree $FILE
FILE=data/input/test_data15
./dist_sweep $FILE
./qtree $FILE
FILE=data/input/test_data19
./dist_sweep $FILE
./qtree $FILE
rm dist_sweep
rm qtree


