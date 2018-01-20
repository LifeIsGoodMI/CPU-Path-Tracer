#pragma once

#include "GameObject.h"

class Plane : public GameObject
{
public:
	Vector3 n;
	double d;

	Plane();
	Plane(Vector3 n, double d);
	using GameObject::GameObject;

	double Intersection(const Ray & ray) const;
	Vector3 GetNormal(const Vector3&) const;
};
