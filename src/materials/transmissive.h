#ifndef TRANSMISSIVE_H
#define TRANSMISSIVE_h

#include "material.h"

class Transmissive : public Material
{
public:

	Transmissive() = delete;
	Transmissive(const double eta, const Vector3D color_);

	virtual Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,
		const Vector3D &wi) const;
	virtual bool hasSpecular() const;
	virtual bool hasTransmission() const;
	virtual bool hasDiffuseOrGlossy() const;
	virtual double getEta() const;

private:
	const double eta;
	const Vector3D color; // Used to store the value of shininess coefficient
};

#endif