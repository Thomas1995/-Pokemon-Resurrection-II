#pragma once

struct Size {
	unsigned int width, height;

	Size();
	Size(const unsigned int width, const unsigned int height);
};

/*
	Brings a real value in the [a, b] interval (default is [0.0, 1.0])
*/
double normalize(double x, const double a = 0, const double b = 1);

/*
	Given a real number as first parameter, returns p% of it
*/
double percentage(double x, int p);