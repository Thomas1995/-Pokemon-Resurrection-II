#include "stdafx.h"
#include "utils.h"

Size::Size() {
	this->width = 0;
	this->height = 0;
}

Size::Size(const unsigned int width, const unsigned int height) {
	this->width = width;
	this->height = height;
}