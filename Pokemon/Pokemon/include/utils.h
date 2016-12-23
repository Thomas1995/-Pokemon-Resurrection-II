#pragma once

template <typename T>
struct Size {
	T width, height;

	Size() : width(0), height(0) {}
	Size(const T width, const T height)
		: width(width), height(height) {}
};

template <typename T>
struct Position {
	T X, Y;

	Position() : X(0), Y(0) {}
	Position(const T x, const T y) : X(x), Y(y) {}
};

/*
	Brings a real value in the [a, b] interval (default is [0.0, 1.0])
*/
double normalize(double x, const double a = 0, const double b = 1);

/*
	Given a real number as first parameter, returns p% of it
*/
double percentage(double x, int p);

/*
	Gets a random integer.
*/
int random();

/*
	Gets a random integer between a and b.
*/
int random(int a, int b);

/*
	Gets a random double between a and b.
*/
double random(double a, double b);