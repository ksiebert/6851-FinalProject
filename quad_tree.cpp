#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>  
#include <vector>
#include <ctime>
#include <dirent.h>

using namespace std;

struct Point {
  int x, y;
  string toString() {
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
    return r;
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

class Line {
  public:
    bool dir;
    Point p0, p1;
    const char * toString();
    Line(Point p2, Point p3);
    Line();
    bool in_range(int x);
    bool intersects(Line l);
};

const char * Line::toString() {
  string r = this->p0.toString();
  r += this->p1.toString();
  return r.c_str();
}


Line::Line(Point p2, Point p3) {
  this->p0 = p0;
  this->p1 = p1;
}
Line::Line() {
  this->p0 = Point();
  this->p1 = Point();
}

// Does the number x fall in the range for which this line varies
bool Line::in_range(int x) {
  if (this->dir) {
    return x >= this->p0.y && x <= this->p1.y;
  } else {
    return x >= this->p0.x && x <= this->p1.x;
  }
}

// Does this line intersect line l
bool Line::intersects(Line l) {
  if (this->dir && l.dir) {
    // Parallel, vertical
    if (this->p0.x == l.p0.x) {
      // Same x coordinate
      if (this->in_range(l.p0.y) || this->in_range(l.p1.y) || l.in_range(this->p0.y) || l.in_range(this->p1.y)) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  } else if (!this->dir && !l.dir) {
    // Parallel, horizontal
    if (this->p0.y == l.p0.y) {
      // Same y coordinate
      if (this->in_range(l.p0.x) || this->in_range(l.p1.x) || l.in_range(this->p0.x) || l.in_range(this->p1.x)) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  } else {
    // Perpendicular
    if (this->dir) {
      return this->in_range(l.p0.y) && l.in_range(this->p0.x);
    } else {
      return this->in_range(l.p0.x) && l.in_range(this->p0.y);
    }
  }
}

class Collision {
  Line * l1;
  Line * l2;

  public:
    Collision(Line * l1, Line * l2);
    void print();
    void write(ofstream * myfile);
};

Collision::Collision(Line * l1, Line * l2) {
  this->l1 = l1;
  this->l2 = l2;
}

void Collision::print() {
  cout << "Collision: " << this->l1->toString() << " " << this->l2->toString() << '\n';
}
void Collision::write(ofstream * myfile) {
  (*myfile) << "Collision: " << this->l1->toString() << " " << this->l2->toString() << '\n';
}


class TreeNode {
  Point lower_left;
  int range;
  vector<Line *> lines;
  TreeNode * parent;
  TreeNode * children[4]; // ll, ul, lr, ur
  bool has_children;
  public:
    TreeNode(TreeNode * parent, Point lower_left, int range);
    int point_in_child(Point p);
    int line_in_child(Line l);
    void create_children(int granularity);
    void add_line(Line * l);
    int size();
    void print();
    void print_lines();
    void print_tree();
    void detect_collisions(vector<Collision> * collisions);
    void detect_internal_collisions(vector<Collision> * collisions);
    void detect_parent_collisions(vector<Collision> * collisions);
    void recursively_detect_collisions(vector<Collision> * collisions);
};

void TreeNode::detect_internal_collisions(vector<Collision> * collisions) {
  if (this->size() > 1) {
    vector<Line *>::iterator it1;
    vector<Line *>::iterator it2;
    for (it1 = this->lines.begin(); it1 != this->lines.end(); it1++) {
      for (it2 = it1+1; it2 != this->lines.end(); it2++) {
        if ((*it1)->intersects(**it2)) {
          Collision col = Collision(*it1, *it2);
          collisions->push_back(col);
        }
      }
    }
  }
}

void TreeNode::detect_parent_collisions(vector<Collision> * collisions) {
  TreeNode * ancestor = this;
  while (ancestor->parent) {
    ancestor = ancestor->parent;
    if (ancestor->size() > 0) {
      vector<Line *>::iterator it1;
      vector<Line *>::iterator it2;
      for (it1 = this->lines.begin(); it1 != this->lines.end(); it1++) {
        for (it2 = ancestor->lines.begin(); it2 != ancestor->lines.end(); it2++) {
          if ((*it1)->intersects(**it2)) {
            Collision col = Collision(*it1, *it2);
            collisions->push_back(col);
          }
        }
      }
    }
  }
}

void TreeNode::recursively_detect_collisions(vector<Collision> * collisions) {
    this->detect_internal_collisions(collisions);
    this->detect_parent_collisions(collisions);
    if (this->has_children) {
      for (int i = 0; i < 4; i++) {
        this->children[i]->recursively_detect_collisions(collisions);
      }
    }
  }

void TreeNode::print() {
  cout << "Treenode rooted at " << this->lower_left.
  toString() << " with range " << this->range << " has " << this->size() << " lines\n";
  if (this->has_children) {
    for (int i = 0; i < 4; i++) {
      cout << "Child rooted at " << this->children[i]->lower_left.toString() << " with range " << this->children[i]->range << " has " << this->children[i]->size() << " lines\n";
    }
  }
  cout << '\n'; 
}

void TreeNode::print_tree() {
  this->print();
  for (int i = 0; i < 4; i++) {
    if (this->children[i]->has_children) {
      this->children[i]->print_tree();
    }
  }
}

TreeNode::TreeNode(TreeNode* parent, Point lower_left, int range) {
  this->lower_left = lower_left;
  this->range = range;
  this->has_children = false;
  this->parent = parent;
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
  return r;
}

int TreeNode::line_in_child(Line l) {
  int c0 = this->point_in_child(l.p0);
  int c1 = this->point_in_child(l.p1);
  if (c0 == c1) {
    return c0;
  } else {
    return -1;
  }
}

void TreeNode::add_line(Line * l) {
  this->lines.push_back(l);
}

void TreeNode::create_children(int granularity) {
  if (this->lines.size() > granularity && this->range > granularity) {
    Point p = Point(this->lower_left.x, this->lower_left.y);
    this->children[0] = new TreeNode(this, p, this->range / 2);

    p = Point(this->lower_left.x + this->range / 2, this->lower_left.y);
    this->children[1] = new TreeNode(this, p, this->range / 2);

    p = Point(this->lower_left.x, this->lower_left.y + this->range / 2);
    this->children[2] = new TreeNode(this, p, this->range / 2);

    p = Point(this->lower_left.x + this->range / 2, this->lower_left.y + this->range / 2);
    this->children[3] = new TreeNode(this, p, this->range / 2);

    vector<Line *>::iterator it1;
    vector<Line *> unclassified_lines;
    for (it1 = this->lines.begin(); it1 != this->lines.end(); it1++) {
      Line * l = *it1;
      int child = this->line_in_child(*l);
      if (child >= 0) {
        this->children[child]->add_line(l);
      } else {
        unclassified_lines.push_back(l);
      }
    }

    if (this->lines.size() != unclassified_lines.size()) {
      this->has_children = true;
    }
    this->lines = unclassified_lines;
    if (this->has_children) {
      for (int i = 0; i < 4; i++) {
        this->children[i]->create_children(granularity);
      }
    }
  } 
}


class QuadTree {
  TreeNode * root;
  int input_size, window_size, granularity;
  string input_file_name;
  Line * input;
  vector<Collision> collisions;
  double duration;
  public:
    QuadTree(string input_file, int window_size, int granularity);
    static Line parse_line(string line_str);
    void generate_tree();
    void detect_collisions();
    void print_tree();
    void add_collision(Collision col);
    void write_collisions_to_file();
    void write_output_to_file();
    void write_data_to_file();
    void print_root_lines();
};

void QuadTree::print_tree() {
  this->root->print_tree();
}

void QuadTree::add_collision(Collision col) {
  this->collisions.push_back(col);
}

void QuadTree::detect_collisions() {
  clock_t start;
  double duration;  
  start = clock();       
  this->root->recursively_detect_collisions(&this->collisions);
  this->duration = (clock() - start ) / (double) CLOCKS_PER_SEC;
}

string output_path_from_input_path(string input_path) {
  return "data/output/qt_" + input_path.substr(input_path.find_last_of("/") + 1, string::npos);
}

string collisions_path_from_input_path(string input_path) {
  return "data/collisions/qt_" + input_path.substr(input_path.find_last_of("/") + 1, string::npos);
}

void QuadTree::write_collisions_to_file() {
  ofstream myfile;
  myfile.open (collisions_path_from_input_path(this->input_file_name));
  vector<Collision>::iterator it;
  for (it = this->collisions.begin(); it != this->collisions.end(); it++) {
    it->write(&myfile);
  }
  myfile.close();
}

void QuadTree::write_data_to_file() {
  ofstream myfile;
  myfile.open (output_path_from_input_path(this->input_file_name));
  myfile <<"qt," << this->input_size << "," << this->collisions.size() << "," << this->duration;
  myfile.close();
}

void QuadTree::write_output_to_file() {
  this->write_collisions_to_file();
  this->write_data_to_file();
}

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

  begin = line_str.find('(', begin + len) + 1;
  len = line_str.find(')', begin) - begin;
  line_data.dir = (atoi(line_str.substr(begin, len).c_str()) > 0);

  return line_data;
}

void TreeNode::print_lines() {
  vector<Line *>::iterator it;
  for (it = this->lines.begin(); it != this->lines.end(); it++) { 
    cout << (*it)->toString() << '\n';
  }
}

void QuadTree::print_root_lines() {
  this->root->print_lines();
}
QuadTree::QuadTree(string input_file, int window_size, int granularity) {
  this->window_size = window_size;
  this->granularity = granularity;
  this->input_file_name = input_file;
  ifstream input;
  string line_str;
  input.open(input_file.c_str());
  if (input.is_open()) {
    getline(input, line_str);
    this->input_size = atoi(line_str.c_str());
    this->input = (Line *)(malloc(this->input_size * sizeof(Line)));
    for (int i = 0; i < this->input_size; i ++) {
      getline(input, line_str);
      this->input[i] = this->parse_line(line_str);
    }
    input.close();
   }
}
  

void QuadTree::generate_tree() {
  Point ll = Point(0, 0);
  this->root = new TreeNode(NULL, ll, this->window_size);
  for (int i = 0; i < this->input_size; i++) {
    this->root->add_line(&this->input[i]);
  }
  this->root->create_children(this->granularity);
}

vector<string> listInputFiles(){
  vector<string> files;
  DIR *pDIR;
  struct dirent *entry;
  if((pDIR=opendir("data/input")) ){
    while((entry = readdir(pDIR))){
      string fn = "data/input/";
      if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
        if (entry->d_name[0]) {
          fn.append(entry->d_name);
          files.push_back(fn);
        }
    }
    closedir(pDIR);
  }
  return files;
}

int main(int argc, char* argv[]) {
  /*
  vector<string> files = listInputFiles();
  vector<string>::iterator it;
  for (it = files.begin(); it != files.end(); it++) { 
    QuadTree qtree = QuadTree(*it, 1000, 100);
    qtree.generate_tree();
    qtree.detect_collisions();
    qtree.write_output_to_file();
  }
  */
  QuadTree qtree = QuadTree(argv[1], 1000, 100);
  qtree.generate_tree();
    qtree.detect_collisions();
    qtree.write_output_to_file();
}
