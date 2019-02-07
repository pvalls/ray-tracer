#include "disk.h"

//Disk is a circular disk of radius r at height h along the z axis

Disk::Disk(const double radius_, const Matrix4x4 &t_, Material *material_, const double height_, const double innerRadius_, const double phiMax_)
	: Shape(t_, material_), radius(radius_), height(height_), innerRadius(innerRadius_), phiMax(phiMax_)
{ }


// Chapter 3 PBRT, page 148
bool Disk::rayIntersect(const Ray &ray, Intersection &its) const
{
// Pass the ray to local object coordinates
	Ray r = worldToObject.transformRay(ray);

//Compute plane intersection for disk //

	//Reject disk intersections for rays parallel to the disk’s plane
	if (ray.d.z == 0) {
		return false;
	}

	//Initialize and obtain tHit assuming plane intersection
	double tHit = (height - ray.o.z) / ray.d.z;

	// Check if tHit is inside ray reach
	if (tHit <= 0 || tHit >= ray.maxT){
		return false;
	}

//See if hit point is inside disk radii and angle //

	// Compute Intersection Point (in local coordinates)
	Vector3D itsPoint = r.o + r.d*tHit;

	//Compute distance to center (0 , 0, height)
	double dist2 = itsPoint.x * itsPoint.x + itsPoint.y * itsPoint.y;

	//false is returned if the distance from the hit to the center of the disk
	//is more than radius or less than the inner radius
	if (dist2 > radius * radius || dist2 < innerRadius * innerRadius) {
		return false;
	}

	// Make sure that the phi value of the hit point is between zero and phiMax
	double phi = std::atan2(itsPoint.y, itsPoint.x);
	if (phi < 0) phi += 2 * M_PI;
	if (phi > phiMax)
		return false;


	// If we arrive here it is because there is an intersection
	// with the tested ray segment!

	// Update the maxT in the ray so as to terminate earlier subsequent
	// intersection tests with other shapes
	ray.maxT = tHit;

	//// Compute Intersection Point (in local coordinates)
	//Vector3D itsPoint = r.o + r.d*tHit;

	// Transform to world coordinates
	its.itsPoint = objectToWorld.transformPoint(itsPoint);

	// Compute the normal at the intersection point (in world coordinates)
	//its.normal = getNormalWorld(its.itsPoint);
	its.normal = Vector3D(0, 0, 1);

	// Store the shape the intersection point lies in
	its.shape = this;

	return true;
}

// Chapter 3 PBRT, page 117
bool Disk::rayIntersectP(const Ray &ray) const
{
	// Pass the ray to local object coordinates
	Ray r = worldToObject.transformRay(ray);

//Compute plane intersection for disk //

	//Reject disk intersections for rays parallel to the disk’s plane
	if (ray.d.z == 0) {
		return false;
	}

	//Initialize and obtain tHit assuming plane intersection
	double tHit = (height - ray.o.z) / ray.d.z;

	// Check if tHit is inside ray reach
	if (tHit <= 0 || tHit >= ray.maxT) {
		return false;
	}

//See if hit point is inside disk radii and angle //

	// Compute Intersection Point (in local coordinates)
	Vector3D itsPoint = r.o + r.d*tHit;

	//Compute distance to center (0 , 0, height)
	double dist2 = itsPoint.x * itsPoint.x + itsPoint.y * itsPoint.y;

	//false is returned if the distance from the hit to the center of the disk
	//is more than radius or less than the inner radius
	if (dist2 > radius * radius || dist2 < innerRadius * innerRadius) {
		return false;
	}

	// Make sure that the phi value of the hit point is between zero and phiMax
	double phi = std::atan2(itsPoint.y, itsPoint.x);
	if (phi < 0) phi += 2 * M_PI;
	if (phi > phiMax)
		return false;

	// If we arrive here it is because there is an intersection
	// with the tested ray segment!

	// Update the maxT in the ray so as to terminate earlier subsequent
	// intersection tests with other shapes
	ray.maxT = tHit;

	return true;
}

