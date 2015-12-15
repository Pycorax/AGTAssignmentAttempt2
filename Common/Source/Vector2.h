#ifndef VECTOR2_H
#define VECTOR2_H

#include <ostream>

using std::ostream;

struct Vector2
{
	static const Vector2 ZERO_VECTOR;

	float x, y;

	virtual bool IsEqual(float a, float b) const;

	Vector2(float a = 0, float b = 0); //default constructor
	Vector2(const Vector2 &rhs); //copy constructor

	void Set(float a, float b); //Set all data

	Vector2& operator=(const Vector2& rhs); //Assignment operator

	Vector2 operator+(const Vector2& rhs) const; //Vector addition
	Vector2 operator-(const Vector2& rhs) const; //Vector subtraction
	Vector2 operator-(void) const; //Unary negation
	Vector2 operator+(const float scalar) const; //Scalar addition
	Vector2 operator*(const float scalar) const; //Scalar multiplication

	Vector2& operator+=(const Vector2& rhs); //Vector addition
	Vector2& operator-=(const Vector2& rhs); //Vector subtraction

	virtual Vector2& operator+=(const float scalar); //Scalar addition
	virtual Vector2& operator-=(const float scalar); //Scalar subtraction

	bool operator==(const Vector2& rhs) const; //Vector comparison
	bool operator!=(const Vector2& rhs) const; //Vector NOT comparison

	virtual float Length(void) const; //Get magnitude
	virtual float LengthSquared(void) const; //Get magnitude
	virtual float Dot(const Vector2& rhs) const; //Dot product
	Vector2& Normalize(void); //Return a copy of this vector, normalized
	Vector2 Normalized(void) const; //Return a copy of this vector, normalized

	friend std::ostream& operator<<(std::ostream& os, Vector2& rhs); //print to ostream
};
#endif
