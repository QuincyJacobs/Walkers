#include "Vector2.h"
#include <iostream>

Vector2::Vector2()
{
}

Vector2::Vector2(double const &x, double const &y) 
{
	this->x = x;
	this->y = y;
}

Vector2 Vector2::operator+(Vector2 vector)
{
	Vector2 addedVector = Vector2((getX() + vector.getX()), (getY() + vector.getY()));
	return addedVector;
}

Vector2 Vector2::operator-(Vector2 vector)
{
	Vector2 subtractedVector = Vector2((getX() - vector.getX()), (getY() - vector.getY()));
	return subtractedVector;
}

double Vector2::getX() {
	return x;
}

void Vector2::setX(double const &x) {
	this->x = x;
}

double Vector2::getY() {
	return y;
}

void Vector2::setY(double const &y) {
	this->y = y;
}

void Vector2::print() {
	std::cout << "X: " << getX() << ";-- Y: " << getY() << std::endl;
}

Vector2::~Vector2()
{
}
