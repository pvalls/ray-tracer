#include "phong.h"
#include "../core/utils.h"

Phong::Phong(Vector3D kd_, Vector3D ks_, double shine_):
	kd(kd_), ks(ks_), shine(shine_)
{}

Vector3D Phong::getReflectance(const Vector3D &n, const Vector3D &wo,
	const Vector3D &wi) const
{
	const Vector3D wr = n*dot(wi,n)*2 - wi;
	Vector3D reflectance = kd*dot(n, wi) + ks*(pow(dot(wo, wr), shine));

	return reflectance;
}

bool Phong::hasSpecular() const
{

	return false;
}

bool Phong::hasTransmission() const
{
	return false;
}

bool Phong::hasDiffuseOrGlossy() const
{

	return true;
}

Vector3D Phong::getDiffuseCoefficient() const
{
	return kd;
}
