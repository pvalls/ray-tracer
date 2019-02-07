#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader(Vector3D bgColor_) :
	Shader(bgColor_)
{ }


Vector3D DirectShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, 
	const std::vector<LightSource*> &lsList) const
{
	//Initialize intersection variable
	Intersection its;

	//Check if there is intersection and if so update its with the info of the closest one
	if (Utils::getClosestIntersection(r, objList, its)) {

		//Initialize color with zeros
		Vector3D color;

		//Initialize vector omega out as inverse of the ray
		Vector3D wo = -r.d;

		//Obtain object material
		const Shape *objShape = its.shape;
		const Material& objMat = objShape->getMaterial();

		//If intersects with Phong object
		if (objMat.hasDiffuseOrGlossy()) {

			//For every Point Light Source in the List
			for (int s = 0; s < lsList.size(); s++) {

				LightSource *pls = lsList.at(s);

				if (pls->isPoint()) {

					Vector3D wi = pls->getPosition() - its.itsPoint;

					Ray ls = Ray::Ray(its.itsPoint, wi.normalized());

					ls.maxT = wi.length();

					if ((!Utils::hasIntersection(ls, objList)) && (!(dot(its.normal, wi) < 0))) {

						Vector3D L = pls->getIntensity(its.itsPoint);
						Vector3D objReflect = objMat.getReflectance(its.normal, wo.normalized(), wi.normalized());
						color += Utils::multiplyPerCanal(L, objReflect);
					}
					
				}
				if (pls->isArea()) {
			//		color += computeColor(r, objList, *pls->getLightSourceList());
					double Npoints = (*pls->getLightSourceList()).size();

					//Check corners				
					bool allVisible = true;
					bool anyVisible = false;

					for (int k = 0; k < (*pls->getKeyPointList()).size(); k++) {
						LightSource *keypoint = (*pls->getKeyPointList()).at(k);
						Vector3D wi = keypoint->getPosition() - its.itsPoint;
						Ray r = Ray::Ray(its.itsPoint, wi.normalized());
						r.maxT = wi.length();
						bool visibility = !Utils::hasIntersection(r, objList);
						allVisible = allVisible && visibility;
						anyVisible = anyVisible || visibility;
					}

					if (allVisible) {

						for (int s = 0; s < (*pls->getLightSourceList()).size(); s++) {

							LightSource *pls1 = (*pls->getLightSourceList()).at(s);

							Vector3D wi = pls1->getPosition() - its.itsPoint;

							Ray ls = Ray::Ray(its.itsPoint, wi.normalized());

							ls.maxT = wi.length();

							if (!(dot(its.normal, wi) < 0)) {
								Vector3D L = pls1->getIntensity(its.itsPoint);
								Vector3D objReflect = objMat.getReflectance(its.normal, wo.normalized(), wi.normalized());
								color += Utils::multiplyPerCanal(L, objReflect);
							}
						}

					}
					else if (anyVisible) {
						for (int s = 0; s < (*pls->getLightSourceList()).size(); s++) {

							LightSource *pls1 = (*pls->getLightSourceList()).at(s);

							Vector3D wi = pls1->getPosition() - its.itsPoint;

							Ray ls = Ray::Ray(its.itsPoint, wi.normalized());

							ls.maxT = wi.length();

							if ((!Utils::hasIntersection(ls, objList)) && (!(dot(its.normal, wi) < 0))) {
								Vector3D L = pls1->getIntensity(its.itsPoint);
								Vector3D objReflect = objMat.getReflectance(its.normal, wo.normalized(), wi.normalized());
								color += Utils::multiplyPerCanal(L, objReflect);
							}
						}
					}
				}
			}
		}
		//If intersects with mirror object
		if (objMat.hasSpecular()) {
			Vector3D wr = Utils::computeReflectionDirection(r.d, its.normal);
			Ray reflectionRay = Ray::Ray(its.itsPoint, wr, r.depth + 1);
			if (reflectionRay.depth<5)
				color = computeColor(reflectionRay, objList, lsList);
		}
		//If intersects withg transmissive object
		if (objMat.hasTransmission()) {
				double eta = objMat.getEta();

				if (dot(its.normal, wo)<0) {
					eta = 1 / eta;
					its.normal = -its.normal;
				}
				double cosThetaI = dot(its.normal.normalized(), wo.normalized());
				double cosThetaT;

				if (!Utils::isTotalInternalReflection(eta, cosThetaI, cosThetaT)) {
					Vector3D wt = Utils::computeTransmissionDirection(r, its.normal.normalized(), eta, cosThetaI, cosThetaT);
					Ray	refracRay = Ray::Ray(its.itsPoint, wt.normalized(), r.depth + 1);
					color = computeColor(refracRay, objList, lsList);
				}
				else {
					Vector3D wr = Utils::computeReflectionDirection(r.d, its.normal);
					Ray reflectionRay = Ray::Ray(its.itsPoint, wr, r.depth + 1);
					color = computeColor(reflectionRay, objList, lsList);
				}
			}

		return color;
	}

	//If there is no intersection return backround color
	else
		return bgColor;
}