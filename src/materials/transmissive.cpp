#include "transmissive.h"
#include "../core/utils.h"

Transmissive::Transmissive(const double eta_, const Vector3D color_) : 
	eta(eta_), color(color_)
{}

Vector3D Transmissive::getReflectance(const Vector3D &n, const Vector3D &wo,
	const Vector3D &wi) const
{
	return color;
}

double Transmissive::getEta() const
{
	return eta;
}

bool Transmissive::hasSpecular() const
{
	return false;
}

bool Transmissive::hasTransmission() const
{
	return true;
}

bool Transmissive::hasDiffuseOrGlossy() const
{
	return false;
}