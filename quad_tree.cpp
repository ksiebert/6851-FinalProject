#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>  
using namespace std;

struct point {
  int x, y;
  const char * toString() {
    stringstream ss;
    string r = "";
    r += "(";
    ss << this->x;
    r += ss.str();
    r += ",";
    stringstream sss;
    sss << this->y;
    r += sss.str();
    r += ")";
    return r.c_str();
  }
};

struct line {
  point p0, p1;
  bool dir;
  const char * toString() {
    string r = "";
    r += this->p0.toString();
    r += this->p1.toString();
    return r.c_str();
  }
};

class TreeNode {
  point lower_left;
  int range;
  line ** node_lines;
  TreeNode * children[4]; // ll, ul, lr, ur
  public:
    TreeNode(point lower_left, int range, line ** node_lines);
    int point_in_child(point p);
    int line_in_child(line l);
};

int TreeNode::point_in_child(point p) {
  int r = 0;
  if (p.x > this->lower_left.x + (range / 2)) r = r & 0x1; 
  if (p.y > this->lower_left.y + (range / 2)) r = r & 0x2;
  return r;
}

int TreeNode::line_in_child(line l) {
  int c0 = this->point_in_child(l.p0);
  int c1 = this->point_in_child(l.p1);
  if (c0 == c1) return -1;
  else return c0;
}

class QuadTree {
  TreeNode * root;
  int input_size, window_size, granularity;
  line * input;
  public:
    QuadTree(const char * input_file, int window_size, int granularity);
    static line parse_line(string line_str);
  private:
    void generate_tree();
    void _generate_tree();
};

/*
Takes strings of the form "(X0,Y0), (X1,Y1), (dir)" and turns them into lines
*/
line QuadTree::parse_line(string line_str) {
  line line_data;
  int begin, len;

  begin = line_str.find('(', 0) + 1;
  len = line_str.find(',', begin) - begin;
  line_data.p0.x = atoi(line_str.substr(begin, len).c_str());

  begin = line_str.find(',', begin + len) + 1;
  len = line_str.find(')', begin) - begin;
  line_data.p0.y = atoi(line_str.substr(begin, len).c_str());

  begin = line_str.find('(', begin + len) + 1;
  len = line_str.find(',', begin) - begin;
  line_data.p1.x = atoi(line_str.substr(begin, len).c_str());

  begin = line_str.find(',', begin + len) + 1;
  len = line_str.find(')', begin) - begin;
  line_data.p1.y = atoi(line_str.substr(begin, len).c_str());

  begin = line_str.find('(', 2) + 1;
  len = line_str.find(')', 2) - begin;
  line_data.dir = atoi(line_str.substr(begin, len).c_str()) > 0;

  return line_data;
}

QuadTree::QuadTree(const char * input_file, int window_size, int granularity) {
  this->window_size = window_size;
  this->granularity = granularity;
  ifstream input;
  string line_str;
  input.open(input_file);
  
  if (input.is_open()) {
    getline(input, line_str);
    this->input_size = atoi(line_str.c_str());
    this->input = (line *)(malloc(this->input_size * sizeof(line)));
    for (int i = 0; i < this->input_size; i ++) {
      getline(input, line_str);
      this->input[i] = parse_line(line_str);
    }
    input.close();
   }
}
  

void QuadTree::generate_tree() {
  /*
  Point ll;
  ll.x, ll.y = 0;
  TreeNode root = TreeNode(ll, this->window_size, this->input);
  */
}

/*
Helper function for recursively generating a tree
*/ 
void _generate_tree() {
}

/*
void quadtree(int** input, int input_size, int top, int left, int window_size):
  int
  for (int i < input_size; i++) {
    if *input[i]
  quadtree(top, left, size / 2);
  quadtree(top, left + size / 2, size / 2);
  quadtree(top + size / 2, left, size / 2);
  quadtree(top + size / 2, left + size / 2, size / 2);
*/


int main() {
  QuadTree qtree = QuadTree("test_data.txt", 1000000, 1000);
}
