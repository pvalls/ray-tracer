#ifndef CYLINDER_H
#define CYLINDER_H

#include <iostream>
#include <string>

#include "shape.h"
#include "../core/eqsolver.h"

#define _USE_MATH_DEFINES
#include <math.h>

class Cylinder : public Shape
{
public:
	Cylinder() = delete;
	Cylinder(const double radius_, const double zMin_, const double zMax_, const Matrix4x4 &t_, Material *material_);

	Vector3D getNormalWorld(const Vector3D & pt_world) const;

	//virtual bool intersect_base(const Ray & ray, const Vector3D & c, double & t) const;
	virtual bool rayIntersect(const Ray &ray, Intersection &its) const;
	virtual bool rayIntersectP(const Ray &ray) const;

private:
	// radius of the bases
	double radius;
	// height of the cylinder
	//double height;
	// center of the bottom base
	//Vector3D center;
	// center of the top base
	//Vector3D center2;

	double zMin;
	double zMax;
};


#endif // CYLINDER_H
