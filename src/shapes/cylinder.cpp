#include "cylinder.h"
#include <cmath>

Cylinder::Cylinder(const double radius_, const double zMin_, const double zMax_, const Matrix4x4 &t_, Material *material_)
	: Shape(t_, material_), radius(radius_), zMin(zMin_), zMax(zMax_)
{ }


Vector3D Cylinder::getNormalWorld(const Vector3D &pt_world) const
{
	// Transform the point to local coordinates
	//Point3D pt_local = worldToObject.applyTransform(pt_world);
	Vector3D pt_local = worldToObject.transformPoint(pt_world);

	// Normal in local coordinates is defined as the derivative
	Vector3D n(2*pt_local.x, 2*pt_local.y,  0);

	// Transform the normal to world coordinates
	//Normal nWorld = objectToWorld.applyTransform(n);
	// Multiply the normal by the transpose of the inverse
	Matrix4x4 inverseTransposed;
	worldToObject.transpose(inverseTransposed);
	Vector3D nWorld = inverseTransposed.transformVector(n);

	// Check whether applying the transform to a normalized
	// normal allways yields a normalized normal
	return(nWorld.normalized());
}

bool Cylinder::rayIntersect(const Ray &ray, Intersection &its) const
{

	// translate the ray origin
	Ray r = worldToObject.transformRay(ray);

	// Coefficients for the intersection equation
	// got them mathematically intersecting the line equation with the cylinder equation
	double a = r.d.x*r.d.x + r.d.y*r.d.y;
	double b = 2*(r.d.x*r.o.x + r.d.y*r.o.y);
	double c = r.o.x*r.o.x + r.o.y*r.o.y - pow(radius, 2);

	// Now we need to solve this quadratic equation for t
	EqSolver solver;
	rootValues roots;
	bool hasRoots = solver.rootQuadEq(a, b, c, roots);

	if (!hasRoots)
	{
		return false;
	}

	// Test whether both root values (i.e., the intersection points)
	// are greater or smaller than r.maxT and r.minT, respectivelly

	if (roots.values[0] > ray.maxT || roots.values[1] < ray.minT)
		return false;

	double tHit = roots.values[0];
	if (roots.values[0] < ray.minT) {
		tHit = roots.values[1];
		if (tHit > ray.maxT)
			return false;
	}

	// If we arrive here it is because there is an intersection
	// with the tested ray segment!

	ray.maxT = tHit;

	// Compute Intersection Point (in local coordinates)
	Vector3D itsPoint = r.o + r.d*tHit;

	if (itsPoint.z < zMin || itsPoint.z > zMax) {
		return false;
	}

	// Transform to world coordinates
	its.itsPoint = objectToWorld.transformPoint(itsPoint);

	// Compute the normal at the intersection point (in world coordinates)
	its.normal = getNormalWorld(its.itsPoint);

	// Store the shape the intersection point lies in
	its.shape = this;

	return true;
}

bool Cylinder::rayIntersectP(const Ray & ray) const
{
	// translate the ray origin
	Ray r = worldToObject.transformRay(ray);

	// coefficients for the intersection equation
	// got them mathematically intersecting the line equation with the cylinder equation
	double a = r.d.x*r.d.x + r.d.y*r.d.y;
	double b = 2 * (r.d.x*r.o.x + r.d.y*r.o.y);
	double c = r.o.x*r.o.x + r.o.y*r.o.y - pow(radius, 2);

	// Now we need to solve this quadratic equation for t
	EqSolver solver;
	rootValues roots;
	bool hasRoots = solver.rootQuadEq(a, b, c, roots);

	if (!hasRoots)
	{
		return false;
	}

	// Test whether both root values (i.e., the intersection points)
	// are greater or smaller than r.maxT and r.minT, respectivelly
	if (roots.values[0] > ray.maxT || roots.values[1] < ray.minT)
		return false;

	// We initialize the tHit for case 1
	double tHit = roots.values[0];
	// We check where we could possibly be in case 2 or 3
	if (roots.values[0] < ray.minT) {
		// If so, then we set tHit to t1 (hoping we are in case 2!)
		tHit = roots.values[1];
		// If we are in case 3, then return false (meaning there is no intersection detected
		if (tHit > ray.maxT)
			return false;
	}

	// Compute Intersection Point (in local coordinates)
	Vector3D itsPoint = r.o + r.d*tHit;

	if (itsPoint.z < zMin || itsPoint.z > zMax) {
		return false;
	}


	ray.maxT = tHit;

	return true;
}
