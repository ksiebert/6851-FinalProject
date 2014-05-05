#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>  
#include <vector>

using namespace std;

class Point {
	int x, y;
	public:
		Point(int x, int y);
		Point();
		void toString();
};

Point::Point(int x, int y) {
	this->x = x;
	this->y = y;
}

Point::Point() {
	this->x = -1;
	this->y = -1;
}


void Point::toString() {
	cout << "(" << this->x << ", " << this->y << ")";
}


class Line {
	Point p1; 
	Point p2;
	public:
		Line(Point p1, Point p2);
		Line();
		void toString();
};

Line::Line(Point p1, Point p2) {
	this->p1 = p1;
	this->p2 = p2;
}

Line::Line() {
	this->p1 = Point();
	this->p2 = Point();
}

void Line::toString() {
	cout << "{";
	this->p1.toString();
	cout << ", ";
	this->p2.toString();
	cout << "}\n";
}

int main() {
	Line line = Line(Point(1,2),Point(3,4));
	line.toString();
	return 1;
}

