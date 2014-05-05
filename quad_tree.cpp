#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>  
#include <vector>

using namespace std;

struct Point {
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
  Point(int x, int y) {
    this->x = x;
    this->y = y;
  }
  Point() {
    this->x = -1;
    this->y = -1;
  }
};

struct Line {
  Point p0, p1;
  bool dir;
  const char * toString() {
    string r = "";
    r += this->p0.toString();
    r += this->p1.toString();
    return r.c_str();
  }
  Line(Point p2, Point p3) {
    this->p0 = p0;
    this->p1 = p1;
  }
  Line() {
    this->p0 = Point();
    this->p1 = Point();
  }
};

class TreeNode {
  Point lower_left;
  int range;
  vector<Line *> lines;
  TreeNode * children[4]; // ll, ul, lr, ur
  public:
    TreeNode(Point lower_left, int range);
    int point_in_child(Point p);
    int line_in_child(Line l);
    void create_children(int granularity);
    void add_line(Line * l);
    int size();
};

TreeNode::TreeNode(Point lower_left, int range) {
  this->lower_left = lower_left;
  this->range = range;
  //cout << "Creating treenode rooted at " << this->lower_left.toString() << " with range " << this->range << '\n';
}

int TreeNode::size() {
  return this->lines.size();
}

int TreeNode::point_in_child(Point p) {
  int r = 0;
  if (p.x > (this->lower_left.x + (this->range / 2))) {
    r |= 0x1; 
  }
  if (p.y > (this->lower_left.y + (this->range / 2))) {
    r |= 0x2;
  }
  //cout << "Point " << p.toString() << "is in child " << r << " of node rooted at " << this->lower_left.toString() << '\n';
  return r;
}

int TreeNode::line_in_child(Line l) {
  int c0 = this->point_in_child(l.p0);
  int c1 = this->point_in_child(l.p1);
  if (c0 == c1) {
    //cout << "Line " << l.toString() << "is in child " << c0 << " of node rooted at " << this->lower_left.toString() << '\n';
    return c0;
  } else {
    //cout << "Line " << l.toString() << "is not in any child of node rooted at " << this->lower_left.toString() << '\n';
    return -1;
  }
}

void TreeNode::add_line(Line * l) {
  lines.push_back(l);
}

void TreeNode::create_children(int granularity) {
  //cout << "Running create children on node that has " << this->size() << "lines\n";
  cout << "Child rooted at " << this->lower_left.toString() << " with range " << this->range << "has " << this->size() << " lines \n";

  if (this->lines.size() > granularity && this->range > granularity) {
    Point p = Point(this->lower_left.x, this->lower_left.y);
    this->children[0] = new TreeNode(p, this->range / 2);

    p = Point(this->lower_left.x + this->range / 2, this->lower_left.y);
    this->children[1] = new TreeNode(p, this->range / 2);

    p = Point(this->lower_left.x, this->lower_left.y + this->range / 2);
    this->children[2] = new TreeNode(p, this->range / 2);

    p = Point(this->lower_left.x + this->range / 2, this->lower_left.y + this->range / 2);
    this->children[3] = new TreeNode(p, this->range / 2);

    vector<Line *>::iterator it1;
    for (it1 = this->lines.begin(); it1 != this->lines.end(); it1++) {
      Line * l = *it1;
      int child = this->line_in_child(*l);
      if (child >= 0) {
        this->children[child]->add_line(l);
      } else {
        cout << "Line " << *l.toString() << " doesn't fit in any children of node rooted at " << this->lower_left.toString() << " with range " << this->range << '\n';
    }
    for (int i = 0; i < 4; i++) {
      this->children[i]->create_children(granularity);
      //cout << "Child rooted at " << this->lower_left.toString() << "has " << this->children[i]->size() << " lines \n";
    }
  } 
}

class QuadTree {
  TreeNode * root;
  int input_size, window_size, granularity;
  Line * input;
  public:
    QuadTree(const char * input_file, int window_size, int granularity);
    static Line parse_line(string line_str);
    void generate_tree();
};

/*
Takes strings of the form "(X0,Y0), (X1,Y1), (dir)" and turns them into lines
*/
Line QuadTree::parse_line(string line_str) {
  Line line_data = Line();
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
    this->input = (Line *)(malloc(this->input_size * sizeof(Line)));
    for (int i = 0; i < this->input_size; i ++) {
      getline(input, line_str);
      this->input[i] = parse_line(line_str);
    }
    input.close();
   }
}
  

void QuadTree::generate_tree() {
  Point ll = Point(0, 0);
  TreeNode root = TreeNode(ll, this->window_size);
  for (int i = 0; i < this->input_size; i++) {
    root.add_line(&this->input[i]);
  }
  cout << "Root has " << root.size() << "lines\n";
  root.create_children(this->granularity);
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
  QuadTree qtree = QuadTree("test_data.txt", 1000000, 100);
  qtree.generate_tree();
}
