#pragma once


struct Vector3
{
	double x, y, z;

	Vector3();
	Vector3(double, double, double);

#pragma region Operator overloads
	Vector3 operator + (const Vector3 &rhs) const;
	Vector3 operator - (const Vector3 &rhs) const;
	Vector3 operator * (double rhs) const;
	Vector3 operator * (const Vector3 & rhs) const;
	Vector3 operator / (double rhs) const;
	Vector3 operator / (Vector3 rhs) const;
#pragma endregion

	double Magnitude();
	Vector3& Normalized();
	
	static double Dot(const Vector3 &lhs, const Vector3 &rhs);
	static Vector3 Cross(const Vector3 &lhs, const Vector3 &rhs);
};
