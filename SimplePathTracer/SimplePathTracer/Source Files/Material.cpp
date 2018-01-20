#include "Material.h"

Material::Material() { emission = 0; type = 1; }
Material::Material(Vector3 color, double emission, int type) : color(color), emission(emission), type(type) {}
