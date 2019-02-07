#ifndef AREALIGHTSOURCE_H
#define AREALIGHTSOURCE_H

#include "lightsource.h"
#include "pointlightsource.h"

class AreaLightSource : public LightSource
{
public:
	AreaLightSource() = delete;
	AreaLightSource(Vector3D pos_, Vector3D intensity_, Vector3D v1_, Vector3D v2_, double sizeLightSource_, double pointSpacing_);

	Vector3D getPosition() const;
	Vector3D getIntensity(const Vector3D &dir) const;
	std::vector<LightSource*>* getLightSourceList() const;
	std::vector<LightSource*>* getKeyPointList() const;

	bool isPoint() const;
	bool isArea() const;

private:
	Vector3D v1;
	Vector3D v2;
	Vector3D norm;
	double sizeLightSource;
	double pointsSpacing;

	//Declare Pointer to vector of LightSources
	std::vector<LightSource*> *lightSourceList;
	std::vector<LightSource*> *keyPointList;
};

#endif // AREALIGHTSOURCE_H
