#include "pointlightsource.h"

PointLightSource::PointLightSource(Vector3D pos_, Vector3D intensity_) :
    LightSource(pos_,intensity_)
{ }

Vector3D PointLightSource::getPosition() const
{
	return pos;
}

Vector3D PointLightSource::getIntensity(const Vector3D &p) const
{
    double sqDistance = (p - pos).lengthSq();

    Vector3D result = intensity / sqDistance;
    return result;
}

bool PointLightSource::isPoint() const
{
	return true;
}

bool PointLightSource::isArea() const
{
	return false;
}

std::vector<LightSource*>* PointLightSource::getLightSourceList() const
{
	return new std::vector<LightSource*>;
}

std::vector<LightSource*>* PointLightSource::getKeyPointList() const
{
	return new std::vector<LightSource*>;
}