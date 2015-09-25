#pragma once
#include "shader.h"

class ShaderProgram
{
	Shader vertex,fragment,geom,geom2,geom_;
public:
	unsigned int programObject;
	
	ShaderProgram();
	~ShaderProgram();
	void init(const char* vName, const char* fName);
};
