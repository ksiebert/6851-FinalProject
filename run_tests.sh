rm data/input/*
rm data/output/*
rm data/collisions/*
python generate_lines.py
rm qtree
g++ -o qtree quad_tree.cpp
./qtree
python create_graphs.py
