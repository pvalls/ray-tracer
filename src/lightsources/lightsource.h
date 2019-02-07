#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H


#include "../core/vector3d.h"
#include <vector>

// To start, let this be the interface of a point light source
// Then, make this an abstract class from which we can derive:
//   - omnidirectional uniform point light sources
//   - area light sources
class LightSource
{
public:
	LightSource() = delete;
	LightSource(Vector3D pos_, Vector3D intensity_);

	// Getters
	virtual Vector3D getPosition() const = 0;
	virtual Vector3D getIntensity(const Vector3D &dir) const = 0;
	virtual std::vector<LightSource*>* getLightSourceList() const = 0;
	virtual std::vector<LightSource*>* getKeyPointList() const = 0;
	virtual bool isPoint() const = 0;
	virtual bool isArea() const = 0;


protected:
	Vector3D pos;
	Vector3D intensity; // (unity: watts/sr)

};

#endif // LIGHTSOURCE_H
