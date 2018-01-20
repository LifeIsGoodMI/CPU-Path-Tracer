#include "Vector3.h"
#include <math.h>


Vector3::Vector3() { x = 0; y = 0; z = 0; }
Vector3::Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}


#pragma region Operator overloads

Vector3 Vector3::operator+(const Vector3 & rhs) const
{
	return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector3 Vector3::operator-(const Vector3 & rhs) const
{
	return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vector3 Vector3::operator*(double rhs) const
{
	return Vector3(x * rhs, y * rhs, z * rhs);
}


Vector3 Vector3::operator*(const Vector3 & rhs) const
{
	return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
}


Vector3 Vector3::operator/(double rhs) const
{
	return Vector3(x / rhs, y / rhs, z / rhs);
}


Vector3 Vector3::operator/(Vector3 rhs) const
{
	return Vector3(x / rhs.x, y / rhs.y, z / rhs.z);
}

#pragma endregion



double Vector3::Magnitude()
{
	double result = x * x + y * y + z * z;
	return sqrt(result);
}

Vector3 & Vector3::Normalized()
{
	return *this = *this * (1 / Magnitude());
}


double Vector3::Dot(const Vector3 &lhs, const Vector3 &rhs)
{
	double result = lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	return result;
}

Vector3 Vector3::Cross(const Vector3 &lhs, const Vector3 &rhs)
{
	double x = lhs.y * rhs.z - lhs.z * rhs.y;
	double y = lhs.z * rhs.x - lhs.x * rhs.z;
	double z = lhs.x * rhs.y - lhs.y * rhs.x;
	return Vector3(x, y, z);
}
