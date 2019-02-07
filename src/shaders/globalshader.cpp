#include "globalshader.h"
#include "../core/utils.h"
#include "../core/hemisphericalsampler.h"
#define _USE_MATH_DEFINES // for C  
#include <math.h>  

GlobalShader::GlobalShader(Vector3D bgColor_,Vector3D at_,int nSamples_, int maxDepth_):
	Shader(bgColor_),at(at_),nSamples(nSamples_), maxDepth(maxDepth_)
{ }


Vector3D GlobalShader::computeColor(const Ray &r, const std::vector<Shape*> &objList,
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
			Vector3D Lind;
			if (r.depth == 0) {
				for (int i = 0; i < nSamples; i++) {
					HemisphericalSampler sampler;
					Vector3D wi = sampler.getSample(its.normal);
					Ray secondaryRay = Ray::Ray(its.itsPoint, wi.normalized(), r.depth + 1);
					Vector3D Li = computeColor(secondaryRay, objList, lsList);
					Vector3D objReflect = objMat.getReflectance(its.normal, wo.normalized(), wi.normalized());
					Lind += Utils::multiplyPerCanal(Li,objReflect);
				}
				Lind = Lind / (2*M_PI*nSamples);
			}
			if(r.depth == maxDepth)
				Lind = Utils::multiplyPerCanal(at,objMat.getDiffuseCoefficient());
			if(r.depth>0 && r.depth<maxDepth){
				Ray normalRay = Ray::Ray(its.itsPoint, its.normal, r.depth + 1);
				Vector3D Lnorm = computeColor(normalRay, objList, lsList);
				Vector3D rn = objMat.getReflectance(its.normal, wo.normalized(), its.normal);
				Lnorm = Utils::multiplyPerCanal(Lnorm, rn);

				Vector3D wr = Utils::computeReflectionDirection(r.d, its.normal);
				Ray reflectionRay = Ray::Ray(its.itsPoint, wr, r.depth + 1);
				Vector3D Lr = computeColor(reflectionRay, objList, lsList);
				Vector3D rr = objMat.getReflectance(its.normal, wo.normalized(), its.normal);
				Lr = Utils::multiplyPerCanal(Lr, rr);

				Lind = Vector3D(Lnorm.x+Lr.x, Lnorm.y + Lr.y, Lnorm.z + Lr.z) / (2 * M_PI * 2);
			}
			color += Lind;
		}
		//If intersects with mirror object
		if (objMat.hasSpecular()) {
			Vector3D wr = Utils::computeReflectionDirection(r.d, its.normal);
			Ray reflectionRay = Ray::Ray(its.itsPoint, wr, r.depth);
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
				Ray	refracRay = Ray::Ray(its.itsPoint, wt.normalized(), r.depth);
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