#include "shaderprogram.h"
#include <GL/glew.h>


#include <iostream>
using namespace std;

ShaderProgram::ShaderProgram():	programObject(0)
{
}


ShaderProgram::~ShaderProgram()
{

	programObject=0;
}

void ShaderProgram::init(const char* vName, const char* fName)
{
	GLint success; //local variable to check status
	//load and compile vertex shader
	success = vertex.readAndCompile(vName,GL_VERTEX_SHADER);
	if (!success)
	{
		throw "Vertex Compilation Error";
	}
	//load and compile fragment shader
	success = fragment.readAndCompile(fName,GL_FRAGMENT_SHADER);
	if (!success)
	{
		throw "Fragment Compilation Error";
	}

	//create programObject
	programObject = glCreateProgram();
	//attach shaders
	glAttachShader(programObject, vertex.shaderObject);
	glAttachShader(programObject, fragment.shaderObject);




	
GLint MaxPatchVertices = 0;
glGetIntegerv(GL_MAX_PATCH_VERTICES, &MaxPatchVertices);
printf("Max supported patch vertices %d\n", MaxPatchVertices);  
glPatchParameteri(GL_PATCH_VERTICES, 3);

	
	success = vertex.readAndCompile("geom.geom",GL_TESS_CONTROL_SHADER);
	if (!success)
	{
		throw "TESS Compilation Error";
	}
	success = vertex.readAndCompile("geom2.geom",GL_TESS_EVALUATION_SHADER);
	if (!success)
	{
		throw "TESS2 Compilation Error";
	}
glAttachShader(programObject, geom.shaderObject);
glAttachShader(programObject, geom2.shaderObject);


	glBindFragDataLocation(programObject, GL_NONE,"fragColor");
	//link shaders in program
	glLinkProgram(programObject);
	glGetProgramiv (programObject, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint maxLength = 0;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &maxLength);

		char* errorLog = new char [maxLength];
		glGetProgramInfoLog(programObject, maxLength, &maxLength, &errorLog[0]);
		cout << errorLog << endl;
		delete[] errorLog;
		
		glDetachShader(programObject,vertex.shaderObject);
		glDetachShader(programObject,geom.shaderObject);
		glDetachShader(programObject,geom2.shaderObject);
		glDetachShader(programObject,fragment.shaderObject);
		
		vertex.Release();
		geom.Release();
		geom2.Release();
		fragment.Release();
		
		throw "Link Error";
	}
}
