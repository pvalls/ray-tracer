#ifndef PHONG_H
#define PHONG_h

#include "material.h"

class Phong : public Material
{
public:

	Phong() = delete;
	Phong(const Vector3D kd_, const Vector3D ks_, const double shine_);

	virtual Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,
		const Vector3D &wi) const;
	virtual bool hasSpecular() const;
	virtual bool hasTransmission() const;
	virtual bool hasDiffuseOrGlossy() const;
	Vector3D getDiffuseCoefficient() const;

private:
	Vector3D kd; // Used to store the diffuse color
	Vector3D ks; // Used to store the specular color
	double shine; // Used to store the value of shininess coefficient

};

#endif