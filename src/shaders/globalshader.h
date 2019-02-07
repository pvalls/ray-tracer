#ifndef GLOBALSHADER_H
#define GLOBALSHADER_H

#include "shader.h"

class GlobalShader : public Shader
{
public:
	GlobalShader() = delete;
	GlobalShader(Vector3D bgColor_,Vector3D at_,int nSamples_, int maxDepth_);

	virtual Vector3D computeColor(const Ray &r,
		const std::vector<Shape*> &objList,
		const std::vector<LightSource*> &lsList) const;

private:
	Vector3D at; // Used to store the ambient term color
	int nSamples; // Used to store the number of samples
	int maxDepth;
};

#endif // GLOBALSHADER_H