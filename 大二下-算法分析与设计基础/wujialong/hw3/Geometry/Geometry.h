#pragma once

#include<algorithm>
#include<cmath>
#define sqr(x) ((x)*(x))

const double inf = 1e30;

struct Point {
	double x, y;
	Point(double x = 0, double y = 0) :x(x), y(y) { }
};

double dist(const Point &A, const Point& B);

std::pair<int, int> closest_divide_and_conquer(const Point *p, int n);
std::pair<int, int> closest_brute(const Point *p, int n);