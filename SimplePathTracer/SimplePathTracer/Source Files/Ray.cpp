#include "Ray.h"


Ray::Ray(){}
Ray::Ray(Vector3 origin, Vector3 direction) : origin(origin), direction(direction.Normalized()) {}
