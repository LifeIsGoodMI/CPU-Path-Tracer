#pragma once

#include "GameObject.h"

class Sphere : public GameObject
{
public:
	Vector3 position;
	double radius;

	Sphere();
	Sphere(Vector3 position, double radius);
	using GameObject::GameObject;

	double Intersection(const Ray & ray) const;
	Vector3 GetNormal(const Vector3&) const;
};

