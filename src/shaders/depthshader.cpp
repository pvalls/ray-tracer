#include "depthshader.h"
#include "../core/utils.h"

//Costructor
DepthShader::DepthShader(Vector3D color_, double maxDist_, Vector3D bgColor_):
	color(color_), maxDist(maxDist_), Shader(bgColor_)
{}


Vector3D DepthShader::computeColor(const Ray &r,
	const std::vector<Shape*> &objList,
	const std::vector<LightSource*> &lsList) const
{
	//Initialize intersection variable
	Intersection its;

	//Compute closest intersection and if there is one then compute color with given formula
	if (Utils::getClosestIntersection(r, objList, its))
		return color*(1.0 - r.maxT/maxDist);

	//If there is no intersection (The distance between ray origin
	//and the closest intersection is greater or equal than maxDist)
	// then return backround color
	else
		return bgColor;


}