#ifndef DISK_H
#define DISK_H

#include <iostream>
#include <string>

#include "shape.h"
#include "../core/eqsolver.h"
#define _USE_MATH_DEFINES
#include <math.h>


class Disk : public Shape
{
public:
	Disk() = delete;
	Disk(const double radius_, const Matrix4x4 &t, Material *material_, const double height_, const double innerRadius_, const double phiMax_);

	virtual bool rayIntersect(const Ray &ray, Intersection &its) const;
	virtual bool rayIntersectP(const Ray &ray) const;

private:
	// The center of the sphere in local coordinates is assumed
	// to be (0, 0, 0). To pass to world coordinates just apply the
	// objectToWorld transformation contained in the mother class
	double radius;
	double height;
	double innerRadius;
	double phiMax;
};

std::ostream& operator<<(std::ostream &out, const Disk &s);

#endif // DISK_H
