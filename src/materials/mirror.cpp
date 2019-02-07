#include "mirror.h"
#include "../core/utils.h"

Mirror::Mirror(const Vector3D color_): color(color_)
{}

//Wont be used, although we could implement that the mirror gives more prominence to certain colors
Vector3D Mirror::getReflectance(const Vector3D &n, const Vector3D &wo,
	const Vector3D &wi) const
{
	return color;
}

bool Mirror::hasSpecular() const
{
	return true;
}

bool Mirror::hasTransmission() const
{
	return false;
}

bool Mirror::hasDiffuseOrGlossy() const
{
	return false;
}