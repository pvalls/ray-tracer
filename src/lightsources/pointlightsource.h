#ifndef POINTLIGHTSOURCE_H
#define POINTLIGHTSOURCE_H

#include "lightsource.h"

// To start, let this be the interface of a point light source
// Then, make this an abstract class from which we can derive:
//   - omnidirectional uniform point light sources
//   - area light sources
class PointLightSource : public LightSource
{
public:
    PointLightSource() = delete;
    PointLightSource(Vector3D pos_, Vector3D intensity_);
	std::vector<LightSource*>* getLightSourceList() const;
	std::vector<LightSource*>* getKeyPointList() const;

    // Getters
    Vector3D getPosition() const;
    Vector3D getIntensity(const Vector3D &dir) const;
	bool isPoint() const;
	bool isArea() const;

};

#endif // POINTLIGHTSOURCE_H
