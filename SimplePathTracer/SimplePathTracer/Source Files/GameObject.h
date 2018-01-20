#pragma once

#include "Material.h"
#include "Ray.h"

class GameObject
{
public:
	Material material;

	GameObject();
	GameObject(Material);

	virtual double Intersection(const Ray&) const = 0;
	virtual Vector3 GetNormal(const Vector3&) const = 0;
};

