#pragma once
class Vector2
{
private:
	double x;
	double y;
public:
	Vector2();
	Vector2(double const &x, double const &y);
	Vector2 operator+(Vector2 vector);
	Vector2 operator-(Vector2 vector);
	double getX();
	void setX(double const &x);
	double getY();
	void setY(double const &y);
	void print();

	~Vector2();
};

