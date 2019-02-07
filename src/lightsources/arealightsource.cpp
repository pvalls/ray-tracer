#include "arealightsource.h"

AreaLightSource::AreaLightSource(Vector3D pos_, Vector3D intensity_, Vector3D v1_, Vector3D v2_, double sizeLightSource_, double pointsSpacing_) :
	LightSource(pos_,intensity_), v1(v1_.normalized()), v2(v2_.normalized()), norm(cross(v1, v2)), sizeLightSource(sizeLightSource_), pointsSpacing(pointsSpacing_)
{ 
	//Declare lightSourceList
	lightSourceList = new std::vector<LightSource*>;
	keyPointList = new std::vector<LightSource*>;

	//The 
	double Npointls = pow(sizeLightSource / pointsSpacing, 2);

	//Any point P in the plane is defined by the vector equation of the plane, P = pos + A*v1 + B*v2
	for (double A = -sizeLightSource/2 ; A < sizeLightSource/2; A += pointsSpacing) {
		for (double B = - sizeLightSource/2; B < sizeLightSource/2; B+=pointsSpacing) {
			Vector3D P = pos + (v1*A) + (v2*B);
			LightSource *pls = new PointLightSource(P,intensity/Npointls);
			lightSourceList->push_back(pls);
		}
	}

	//Compute keyPoint locations
	for (double A = -sizeLightSource / 2; A <= sizeLightSource / 2; A += sizeLightSource) {
		for (double B = -sizeLightSource / 2; B < sizeLightSource / 2; B += 2*pointsSpacing) {
			Vector3D P = pos + (v1*A) + (v2*B);
			LightSource *pls = new PointLightSource(P, intensity / Npointls);
			keyPointList->push_back(pls);
		}
	}
	
	for (double A = -sizeLightSource / 2 + 2*pointsSpacing; A < sizeLightSource / 2 - 2*pointsSpacing; A += 2*pointsSpacing) {
		for (double B = -sizeLightSource / 2; B <= sizeLightSource / 2; B += sizeLightSource) {
			Vector3D P = pos + (v1*A) + (v2*B);
			LightSource *pls = new PointLightSource(P, intensity / Npointls);
			keyPointList->push_back(pls);
		}
	}
	//Include center point as keypoint
	LightSource *pls = new PointLightSource(pos, intensity / Npointls);
	keyPointList->push_back(pls);

}

Vector3D AreaLightSource::getPosition() const
{
	return pos;
}

Vector3D AreaLightSource::getIntensity(const Vector3D &p) const {
	
	double sqDistance = (p - pos).lengthSq();
	Vector3D result = intensity / sqDistance;
	return result;
}

bool AreaLightSource::isPoint() const
{
	return false;
}

bool AreaLightSource::isArea() const
{
	return true;
}

std::vector<LightSource*>* AreaLightSource::getLightSourceList() const 
{
	return lightSourceList;
}

std::vector<LightSource*>* AreaLightSource::getKeyPointList() const
{
	return keyPointList;
}