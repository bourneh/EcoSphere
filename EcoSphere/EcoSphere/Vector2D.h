#ifndef DS_ECOSYSTEM_VECTOR2D
#define DS_ECOSYSTEM_VECTOR2D

class Vector2D
{
public:
	double x, y;
	Vector2D(double x = 0.0, double y = 0.0); static double dot_product(const Vector2D &a, const Vector2D &b);
	double modulus() const;
	Vector2D rotate(double angle) const;
	Vector2D& operator=(const Vector2D &r);
	Vector2D operator-() const;
	Vector2D operator+(const Vector2D &r) const;
	Vector2D operator-(const Vector2D &r) const;
	Vector2D& operator+=(const Vector2D &r);
	Vector2D& operator-=(const Vector2D &r);
	bool operator==(const Vector2D &r) const;
	bool operator!=(const Vector2D &r) const;
	template<typename t> Vector2D operator*(const t &r) const;
	template<typename t> Vector2D& operator*=(const t &r);
};

template<typename t> Vector2D operator*(const t &l, const Vector2D &r);
#endif // DS_ECOSYSTEM_VECTOR2D