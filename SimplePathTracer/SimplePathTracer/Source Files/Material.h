#pragma once

#include "Vector3.h"

class Material
{
public:
	Vector3 color;
	double emission;
	int type;

	Material();
	Material(Vector3, double, int);
};

