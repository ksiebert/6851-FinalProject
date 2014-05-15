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

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////           Point Class             /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
class Point {
	int x, y;
	public:
		Point(int x, int y);
		Point();
		void toString();
		int getY();
};

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


void Point::toString() {
	cout << "(" << this->x << ", " << this->y << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////            Line Class             /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
class Line {
	Point p1; 
	Point p2;
	bool dir;
	public:
		Line(Point p1, Point p2);
		Line();
		void toString();
		void setDirection(bool dir);
		int getY1();
		int getY2();
};

Line::Line(Point p1, Point p2) {
	this->p1 = p1;
	this->p2 = p2;
}

Line::Line() {
	this->p1 = Point();
	this->p2 = Point();
}

int Line::getY1() {
	return this->p1.getY();
}

int Line::getY2() {
	return this->p2.getY();
}

void Line::setDirection(bool dir) {
	this->dir = dir;
}

void Line::toString() {
	cout << "{";
	this->p1.toString();
	cout << ", ";
	this->p2.toString();
	cout << "}\n";
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////        Collision Class            /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
class Collision {
  	Line * l1;
  	Line * l2;

  	public:
   		Collision(Line * l1, Line * l2);
};

Collision::Collision(Line * l1, Line * l2) {
  	this->l1 = l1;
  	this->l2 = l2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////           Plane Class             /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Plane class
// stores all of the line segments that appear in the plane
// stores line segments in two lists, horizontal and vertical line segments
// Uses Distribution sweeping to find intersections between line segments
class Plane {
	Line * vLines;
	int vLineCount;
	Line * hLines;
	int hLineCount;
	vector<Collision> collisions;
	double sort_time;
	
	public:
		Plane(string input_file);
		void parseLine(string line_str);
		vector<Collision> distributionSweep();
		void detectCollisions();
		void writeCollisionsToFile();
		void sortLinesY();
		void quicksort(Line *input, int p, int r);
		int partition(Line *input, int p, int r);
};

// Initialize plane to store all points in place
// Store the lines in the vertical or horizontal list depending on line
Plane::Plane(string input_file) {
	this->vLineCount = 0;
	this->hLineCount = 0;
	ifstream input;
  	string line_str;
  	input.open(input_file.c_str());
   
    // Read in input file
  	if (input.is_open()) {
    	getline(input, line_str);
    	int input_size = atoi(line_str.c_str());
    	this->vLines = (Line *)(malloc(input_size * sizeof(Line)));
    	this->hLines = (Line *)(malloc(input_size * sizeof(Line)));
    	cout << "Input Size: " << input_size;
    	for (int i = 0; i < input_size; i ++) {
      		getline(input, line_str);
      		this->parseLine(line_str);
    	}
    	input.close();

    	// Reallocate memory for vertical and horizontal lines
    	this->vLines = (Line *)realloc(this->vLines, sizeof(Line) * this->vLineCount);
    	this->hLines = (Line *)realloc(this->hLines, sizeof(Line) * this->hLineCount);
   	}

}

// Parses out line information from input file
// Places line in vertical or horizontal line list
void Plane::parseLine(string line_str) {
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

	if (atoi(line_str.substr(begin, len).c_str()) > 0) {
		this->vLines[vLineCount] = line;
		this->vLineCount++;
	} else {
		this->hLines[hLineCount] = line;
		this->hLineCount++;
	}
}

// Sorts both the vertical and horizontal in an increasing order of y value of first point in line
void Plane::sortLinesY() {
	clock_t start;
  	double duration;  
  	start = clock();       
	this->quicksort(this->vLines, 0, this->vLineCount);
	this->quicksort(this->hLines, 0, this->hLineCount);

	// Log time to sort
	this->sort_time = (clock() - start ) / (double) CLOCKS_PER_SEC;
}


// Recursive function implementing distribution sweeping
vector<Collision> Plane::distributionSweep() {
	vector<Collision> out;
	return out;
}

void Plane::detectCollisions() {
	collisions = this->distributionSweep();
}

// Recursive implementation of quicksort algorithm
void Plane::quicksort(Line* input, int p, int r) {
	if ( p < r ) {
        int j = this->partition(input, p, r);        
        this->quicksort(input, p, j-1);
        this->quicksort(input, j+1, r);
    }
}

// Function to find the partition in quicksort
// Addapted to deal with input of list of lines
int Plane::partition(Line* input, int p, int r) {
	int pivot = input[r].getY1();

    while ( p < r ) {
        while ( input[p].getY1() < pivot ) {
            p++;
    	}
        while ( input[r].getY1() > pivot ) {
            r--;
        }
        if ( input[p].getY1() == input[r].getY1() ) {
            p++;
        } else if ( p < r ) {
            Line tmp = input[p];
            input[p] = input[r];
            input[r] = tmp;
        }
    }

    return r;
}


int main() {
	string file = "data/input/test_data0";
	Plane plane = Plane(file);
	plane.sortLinesY();
	return 1;
}



