﻿#include "material.h"

#include <iostream>

Material::Material()
{ }

double Material::getIndexOfRefraction() const
{
    std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
              << std::endl;

    return -1;
}

double Material::getEta() const
{
	return -1;
}

Vector3D Material::getDiffuseCoefficient() const
{
	std::cout<<"Warning!"
		<< "Calling \"Material::getDiffuseCoefficient()\""
		<< "for a non−diffuse or non−glossymaterial"
		<< std::endl;
	return Vector3D(-1);
}