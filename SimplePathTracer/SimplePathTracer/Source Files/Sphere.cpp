#include "Sphere.h"
#include <math.h>

const double Epsilon = 1e-6;

Sphere::Sphere() { radius = 1; }
Sphere::Sphere(Vector3 position, double radius) : position(position), radius(radius) {}


double Sphere::Intersection(const Ray& ray) const
{
	auto delta = ray.origin - position;
	double b = Vector3::Dot((delta * 2), ray.direction);
	double c = Vector3::Dot(delta, delta) - (radius * radius);

	double disc = b * b - 4 * c;
	if (disc < 0)
		return 0;
	else
		disc = sqrt(disc);

	// Always 2 solutions when pulling the square root
	double result1 = -b + disc;
	double result2 = -b - disc;

	return (result2 > Epsilon) ? result2 / 2 : ((result1 > Epsilon) ? result1 / 2 : 0);
}

Vector3 Sphere::GetNormal(const Vector3 &hitPos) const
{
	return (hitPos - position) / radius;
}

