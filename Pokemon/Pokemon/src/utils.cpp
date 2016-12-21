#include "stdafx.h"
#include "utils.h"
#include <assert.h>
#include <time.h>

Size::Size() {
	this->width = 0;
	this->height = 0;
}

Size::Size(const unsigned int width, const unsigned int height) {
	this->width = width;
	this->height = height;
}

double normalize(double x, const double a, const double b) {
	assert(a <= b);
	x = max(x, a);
	x = min(x, b);
	return x;
}

double percentage(double x, int p) {
	return x / 100.0 * p;
}

int random() {
	static bool seed = []() {
		srand(time(NULL));
		return true;
	}();
	
	return rand();
}

int random(int a, int b) {
	assert(a <= b);
	return random() % (b - a + 1) + a;
}

double random(double a, double b) {
	assert(a <= b);
	const int precision = 1e5;
	return (double)random((int)(a * precision), (int)(b * precision)) / precision;
}
