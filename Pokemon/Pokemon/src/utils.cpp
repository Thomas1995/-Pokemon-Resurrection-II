#include "stdafx.h"
#include "utils.h"
#include <assert.h>
#include <time.h>

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
		srand((unsigned int)time(NULL));
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
	const int precision = (int)1e5;
	return (double)random((int)(a * precision), (int)(b * precision)) / precision;
}