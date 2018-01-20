#include "Plane.h"
#include <math.h>

const double Epsilon = 1e-6;

Plane::Plane() { d = 1; }
Plane::Plane(Vector3 n, double d) : n(n), d(d) {}


double Plane::Intersection(const Ray& ray) const
{
	double d0 = Vector3::Dot(n, ray.direction);
	if (d0 != 0) 
	{
		double t = -1 * (((Vector3::Dot(n, ray.origin)) + d) / d0);

		return (t > Epsilon) ? t : 0;
	}
	else 
		return 0;
}

Vector3 Plane::GetNormal(const Vector3 &hitPos) const
{
	return n;
}

