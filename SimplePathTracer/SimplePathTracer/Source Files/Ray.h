#pragma once
#include "Vector3.h"

struct Ray
{
	Vector3 origin, direction;

	Ray();
	Ray(Vector3 origin, Vector3 direction);
};