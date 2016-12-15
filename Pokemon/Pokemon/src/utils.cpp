#include "stdafx.h"
#include "utils.h"
#include <assert.h>

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
