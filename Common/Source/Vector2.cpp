#include "Vector2.h"

#include "MyMath.h"

const Vector2 Vector2::ZERO_VECTOR = Vector2();

bool Vector2::IsEqual(float a, float b) const
{
	return a - b <= Math::EPSILON && b - a <= Math::EPSILON;
}

Vector2::Vector2(float a, float b)
{
	x = a;
	y = b;
}

Vector2::Vector2(const Vector2 &rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
}

void Vector2::Set(float a, float b)
{
	x = a;
	y = b;
}

Vector2 & Vector2::operator=(const Vector2 & rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;

	return *this;
}

Vector2 Vector2::operator+(const Vector2& rhs) const
{
	Vector2 result;

	result.x = this->x + rhs.x;
	result.y = this->y + rhs.y;

	return result;
}

Vector2 Vector2::operator-(const Vector2& rhs) const
{
	Vector2 result;

	result.x = this->x - rhs.x;
	result.y = this->y - rhs.y;

	return result;
}

Vector2 Vector2::operator-(void) const
{
	Vector2 result;

	result.x = -x;
	result.y = -y;

	return result;
}

Vector2 Vector2::operator+(const float scalar) const
{
	Vector2 result;

	result.x = this->x + scalar;
	result.y = this->y + scalar;

	return result;
}

Vector2 Vector2::operator*(const float scalar) const
{
	Vector2 result;

	result.x = this->x * scalar;
	result.y = this->y * scalar;

	return result;
}

Vector2& Vector2::operator+=(const Vector2& rhs)
{
	this->x = this->x + rhs.x;
	this->y = this->y + rhs.y;

	return *this;
}

Vector2 & Vector2::operator-=(const Vector2 & rhs)
{
	this->x = this->x - rhs.x;
	this->y = this->y - rhs.y;

	return *this;
}

Vector2 & Vector2::operator+=(const float scalar)
{
	this->x = this->x + scalar;
	this->y = this->y + scalar;

	return *this;
}

Vector2 & Vector2::operator-=(const float scalar)
{
	this->x = this->x - scalar;
	this->y = this->y - scalar;

	return *this;
}

bool Vector2::operator==(const Vector2& rhs) const
{
	return IsEqual(x, rhs.x) && IsEqual(y, rhs.y);
}

bool Vector2::operator!=(const Vector2& rhs) const
{
	return !IsEqual(x, rhs.x) || !IsEqual(y, rhs.y);
}

float Vector2::Length(void) const
{
	return sqrt(LengthSquared());
}

float Vector2::LengthSquared(void) const
{
	return x * x + y * y;
}

float Vector2::Dot(const Vector2& rhs) const
{
	return (x * rhs.x) + (y * rhs.y);
}

Vector2& Vector2::Normalize(void)
{
	float d = Length();
	if (d <= Math::EPSILON && -d <= Math::EPSILON)
		throw DivideByZero();
	x /= d;
	y /= d;
	return *this;
}

Vector2 Vector2::Normalized(void) const
{
	float d = Length();
	if (d <= Math::EPSILON && -d <= Math::EPSILON)
		throw DivideByZero();
	return Vector2(x / d, y / d);
}

std::ostream& operator<< (std::ostream& os, Vector2& rhs)
{
	os << "[ " << rhs.x << ", " << rhs.y << " ]";
	return os;
}