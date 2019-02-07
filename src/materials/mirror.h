#ifndef MIRROR_H
#define MIRROR_h
#include "material.h"

class Mirror : public Material
{
public:

	Mirror() = delete;
	Mirror(const Vector3D color_);

	virtual Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,
		const Vector3D &wi) const;
	virtual bool hasSpecular() const;
	virtual bool hasTransmission() const;
	virtual bool hasDiffuseOrGlossy() const;

private:
	Vector3D color; // Used to store the value of shininess coefficient

};

#endif#pragma once
