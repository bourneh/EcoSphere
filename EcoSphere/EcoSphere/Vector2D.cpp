#include <cmath>
#include "Vector2D.h"

Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

double Vector2D::dot_product(const Vector2D &a, const Vector2D &b)
{
	return a.x * b.x + a.y * b.y;
}

double Vector2D::modulus() const
{
	return std::sqrt(x*x + y*y);
}

Vector2D Vector2D::rotate(double angle) const
{
	const static double PI = 3.1415926;
	return Vector2D(x * cos(angle * PI / 180.0) - y * sin(angle * PI / 180.0), x * sin(angle * PI / 180.0) + y * cos(angle * PI / 180.0));
}

Vector2D& Vector2D::operator=(const Vector2D &r)
{
	x = r.x;
	y = r.y;
	return (*this);
}
Vector2D Vector2D::operator-() const
{
	return Vector2D(-x, -y);
}

Vector2D Vector2D::operator+(const Vector2D &r) const
{
	return Vector2D(x + r.x, y + r.y);
}

Vector2D Vector2D::operator-(const Vector2D &r) const
{
	return Vector2D(x - r.x, y - r.y);
}

Vector2D& Vector2D::operator+=(const Vector2D &r)
{
	x = x + r.x;
	y = y + r.y;
	return (*this);
}

Vector2D& Vector2D::operator-=(const Vector2D &r)
{
	x = x - r.x;
	y = y - r.y;
	return (*this);
}

template<typename t>
Vector2D& Vector2D::operator*=(const t &r)
{
	x *= r;
	y *= r;
	return (*this);
}

bool Vector2D::operator==(const Vector2D &r) const
{
	return (std::abs(x - r.x) < 0.000000001 && std::abs(y - r.y) < 0.000000001);
}

bool Vector2D::operator!=(const Vector2D &r) const
{
	return !((*this) == r);
}

template<typename t>
Vector2D Vector2D::operator*(const t &r) const
{
	return Vector2D(x * r, y * r);
}

template<typename t>
Vector2D operator*(const t &l, const Vector2D &r)
{
	return Vector2D(r.x * l, r.y * l);
}