#ifndef DIRECTSHADER_H
#define DIRECTSHADER_H

#include "shader.h"

class DirectShader : public Shader
{
public:
	DirectShader() = delete;
	DirectShader(Vector3D bgColor_);

	virtual Vector3D computeColor(const Ray &r,
		const std::vector<Shape*> &objList,
		const std::vector<LightSource*> &lsList) const;

};

#endif // DIRECTSHADER_H