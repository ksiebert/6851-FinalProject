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

class Point {
	int x, y;
	public:
		Point(int x, int y);
		Point();
		string toString();
		int getY();
		int getX();
};

class Line {
	Point p1; 
	Point p2;
	int dir;
	public:
		Line(Point p1, Point p2);
		Line();
		const char * toString();
		void setDirection(int dir);
		Point* getP1();
		Point* getP2();
		int getDir();
		bool intersect(Line line2);
};

class Collision {
  	Line *l1;
  	Line *l2;

  	public:
   		Collision(Line * l1, Line * l2);
   		Collision();
   		void write(ofstream * myfile);
};



string output_path_from_input_path(string input_path);
string collision_path_from_input_path(string input_path);
Line parseLine(string line_str);
void quicksortX(Line* input, int p, int r);
int partitionX(Line* input, int p, int r);
void quicksortY(Line* input, int p, int r);
int partitionY(Line* input, int p, int r);
void distributionSweep(vector<Line*> lines, vector<Line> overflowLines, int numLines, int numOverflowLines, vector<Collision> * collisions, int basecase, int previousSize);
bool xComparison(Line l1, Line l2);
bool yComparison(Line l1, Line l2);

bool yComparison(Line l1, Line l2) {
	return l1.getP1()->getY() < l2.getP1()->getY();
}
bool xComparison(Line l1, Line l2) {
	return l1.getP1()->getX() < l2.getP1()->getX();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////           Point Class             /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


Point::Point(int x, int y) {
	this->x = x;
	this->y = y;
}

Point::Point() {
	this->x = -1;
	this->y = -1;
}

int Point::getY() {
	return this->y;
}

int Point::getX() {
	return this->x;
}


string Point::toString() {
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

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////            Line Class             /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


Line::Line(Point p1, Point p2) {
	this->p1 = p1;
	this->p2 = p2;
}

Line::Line() {
	this->p1 = Point();
	this->p2 = Point();
}

Point* Line::getP1() {
	return &(this->p1);
}

Point* Line::getP2() {
	return &(this->p2);
}

void Line::setDirection(int dir) {
	this->dir = dir;
}

const char * Line::toString() {
	string r = this->p1.toString();
  	r += this->p2.toString();
  	return r.c_str();
}

int Line::getDir() {
	return this->dir;
}
bool Line::intersect(Line l) {
	int p1x = this->p1.getX();
	int p1y = this->p1.getY();
	int p2x = this->p2.getX();
	int p2y = this->p2.getY();
	int p3x = l.getP1()->getX();
	int p3y = l.getP1()->getY();
	int p4x = l.getP2()->getX();
	int p4y = l.getP2()->getY();
	if (this->dir && l.getDir()) {
	    // Parallel, vertical
	    if (p1x == p3x) {
		    // Same x coordinate
			return (p1y >= p3y && p1y <= p4y) || (p2y >= p3y && p2y <= p4y);
		} else {
			return false;
		}
  	} else if (!this->dir && !l.getDir()) {
    	// Parallel, horizontal
    	if (p1y == p3y) {
      		// Same y coordinate
      		return (p1x >= p3x && p1x <= p4x) || (p2x >= p3x && p2x <= p4x);
    	} else {
      		return false;
    	}
  	} else {
    	// Perpendicular
    	if (this->dir) {
      		return p1x >= p3x && p1x <= p4x && p3y >= p1y && p3y <= p2y;
    	} else {
      		return p1y >= p3y && p1y <= p4y && p3x >= p1x && p3x <= p2x;
    	}
  	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////        Collision Class            /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
Collision::Collision(Line * l1, Line * l2) {
  	this->l1 = l1;
  	this->l2 = l2;
}

Collision::Collision() {
	this->l1 = NULL;
	this->l2 = NULL;
}

void Collision::write(ofstream * myfile) {
  (*myfile) << "Collision: " << this->l1->toString() << " " << this->l2->toString() << '\n';
}


// Parses out line information from input file
// Places line in vertical or horizontal line list
Line parseLine(string line_str) {
	Line line = Line();
	int begin, len, x, y;
	Point p1, p2;

	begin = line_str.find('(', 0) + 1;
	len = line_str.find(',', begin) - begin;
	x = atoi(line_str.substr(begin, len).c_str());

	begin = line_str.find(',', begin + len) + 1;
	len = line_str.find(')', begin) - begin;
	y = atoi(line_str.substr(begin, len).c_str());
	p1 = Point(x,y);


	begin = line_str.find('(', begin + len) + 1;
	len = line_str.find(',', begin) - begin;
	x = atoi(line_str.substr(begin, len).c_str());

	begin = line_str.find(',', begin + len) + 1;
	len = line_str.find(')', begin) - begin;
	y = atoi(line_str.substr(begin, len).c_str());
	p2 = Point(x,y);

	line = Line(p1,p2);

	begin = line_str.find('(', begin + len) + 1;
	len = line_str.find(')', begin) - begin;
	line.setDirection(atoi(line_str.substr(begin, len).c_str()));
	
	return line;
}

// Recursive implementation of quicksort algorithm
void quicksortY(Line* input, int p, int r) {
	if ( p < r ) {
        int j = partitionY(input, p, r);        
        quicksortY(input, p, j-1);
        quicksortY(input, j+1, r);
    }
}

void quicksortX(Line* input, int p, int r) {
	if ( p < r ) {
        int j = partitionX(input, p, r);        
        quicksortX(input, p, j-1);
        quicksortX(input, j+1, r);
    }
}

int partitionY(Line* input, int p, int r) {
	int pivot = input[r].getP1()->getY();

    while ( p < r ) {
        while ( input[p].getP1()->getY() < pivot ) {
            p++;
    	}
        while ( input[r].getP1()->getY() > pivot ) {
            r--;
        }
        if ( input[p].getP1()->getY() == input[r].getP1()->getY() ) {
            p++;
        } else if ( p < r ) {
            Line tmp = input[p];
            input[p] = input[r];
            input[r] = tmp;
        }
    }

    return r;
}

int partitionX(Line* input, int p, int r) {
	int pivot = input[r].getP1()->getX();

    while ( p < r ) {
        while ( input[p].getP1()->getX() < pivot ) {
            p++;
    	}
        while ( input[r].getP1()->getX() > pivot ) {
            r--;
        }
        if ( input[p].getP1()->getX() == input[r].getP1()->getX() ) {
            p++;
        } else if ( p < r ) {
            Line tmp = input[p];
            input[p] = input[r];
            input[r] = tmp;
        }
    }

    return r;
}

void distributionSweep(Line* lines, vector<Line> overflowLines, int numLines, int numOverflowLines, vector<Collision> * collisions, int basecase, int previousSize) {
	// Problem is small enough, solve it
	if (numLines <= basecase || numLines == previousSize) {
		// Sort in Y then look for collisions

		overflowLines.insert(overflowLines.end(),lines, lines+numLines);
		sort(overflowLines.begin(), overflowLines.end(), yComparison);
		vector<Line> activeLines;
		for (vector<Line>::iterator line = overflowLines.begin(); line != overflowLines.end(); line++) {
			vector<Line> activeHolder = activeLines;
			activeLines.clear();
			for (vector<Line>::iterator it = activeHolder.begin(); it != activeHolder.end(); it++) {
				if ((*line).intersect(*it)) {
					// cout << "COLLISION: ";
					// cout << (*line).toString();
					// cout << (*it).toString();
					// cout << "\n";
					collisions->push_back(Collision(&(*line), &(*it)));
					activeLines.push_back(*it);
				} else {
					// Check to see if active line should still be active
					if ((*it).getP2()->getY() >= (*line).getP1()->getY()) {
						activeLines.push_back(*it);
					}
				}
			}

			// IF line is vertical add to active lines
			if ((*line).getDir()) {
				activeLines.push_back(*line);
			}
		}


	} else {
		int median_index = (numLines/2) - (numOverflowLines/2);
		if (median_index < 0) {
			median_index = 0;
		}
		int median = lines[median_index].getP1()->getX();
		int numRightLines = numLines - median_index;

		// Check for lines that start left of median and end to the right of it
		vector<Line> rightOverflowLines;
		for (int i = 0; i < median_index; i++) {
			if (lines[i].getP2()->getX() > median) {
				rightOverflowLines.push_back(lines[i]);
			}
		}

		for (vector<Line>::iterator it = overflowLines.begin(); it != overflowLines.end(); it++) {
			if ((*it).getP2()->getX() > median) {
				rightOverflowLines.push_back(*it);
			}
		}
		

		distributionSweep(lines, overflowLines, median_index, numOverflowLines, collisions, basecase, numLines);
		distributionSweep(lines + median_index, rightOverflowLines, numRightLines, rightOverflowLines.size(), collisions, basecase, numLines);
		return;
	}
}

string output_path_from_input_path(string input_path) {
  	return "data/output/ds_" + input_path.substr(input_path.find_last_of("/") + 1, string::npos);
}

string collisions_path_from_input_path(string input_path) {
  	return "data/collisions/ds_" + input_path.substr(input_path.find_last_of("/") + 1, string::npos);
}


int main(int argc, char* argv[]) {
	string file = argv[1];
	int basecase = atoi(argv[2]);
	ifstream input;
  	string line_str;
  	int input_size;
  	vector<Line> lines;

  	// Read in lines
  	input.open(file.c_str());
  	if (input.is_open()) {
    	getline(input, line_str);
    	input_size = atoi(line_str.c_str());
    	for (int i = 0; i < input_size; i ++) {
      		getline(input, line_str);
      		lines.push_back(parseLine(line_str));
    	}
    	input.close();
   	}

   	// Find collisions in lines
   	clock_t start;
  	double duration;  
  	start = clock();       
   	sort(lines.begin(), lines.end(), xComparison);
   	Line *lines_a = &lines[0];
   	vector<Collision> collisions;
   	vector<Line> overflow;
   	distributionSweep(lines_a, overflow, input_size, 0, &collisions, basecase, -1);
   	duration = (clock() - start ) / (double) CLOCKS_PER_SEC;
   	
   	// Write to output
   	// 	ofstream myfile;
  	// myfile.open (collisions_path_from_input_path(file));
  	// vector<Collision>::iterator it;
  	// for (it = collisions.begin(); it != collisions.end(); it++) {
   	// 		it->write(&myfile);
  	// }
  	// myfile.close();

  	ofstream myfile2;
  	myfile2.open (output_path_from_input_path(file));
  	myfile2 <<"ds," << input_size << "," << collisions.size() << "," << duration;
  	myfile2.close();
	return 1;
}



