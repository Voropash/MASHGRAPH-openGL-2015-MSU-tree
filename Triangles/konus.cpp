#include "konus.h"
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <iostream>
using namespace std;


Konus::Konus(void)
{
}


Konus::~Konus(void)
{
}

void Konus::initData()
{
	if (pData)
	{
		delete[] pData;
		delete[] pIndices;
	}


	//number of points
	dataCount = 3; 
	//number of triangles
	unsigned int nTriangles = 1;
	//number of indices
	indicesCount = 3;

	pData = new VertexData [dataCount];
	pIndices = new unsigned int [indicesCount];
	
	//fill in pData array

	
			float fi = M_PI*2/3; //from 0 to 360 degrees
			float xPos = cos(fi);
			float yPos = sin(fi);
			
			pData[0].pos = glm::vec3(1*xPos, 0,1*yPos);
			pData[0].nor = glm::vec3(1*xPos, 0,1*yPos);
			pData[0].tex = glm::vec2((xPos+1)/2, (yPos+1)/2);		

			 fi = M_PI*4/3; //from 0 to 360 degrees
			 xPos = cos(fi);
			 yPos = sin(fi);
			
			pData[1].pos = glm::vec3(1*xPos, 0,1*yPos);
			pData[1].nor = glm::vec3(1*xPos, 0,1*yPos);
			pData[1].tex = glm::vec2((xPos+1)/2, (yPos+1)/2);	


			 fi = M_PI*0/3; //from 0 to 360 degrees
			 xPos = cos(fi);
			 yPos = sin(fi);
			
			pData[2].pos = glm::vec3(1*xPos, 0,1*yPos);
			pData[2].nor = glm::vec3(1*xPos, 0,1*yPos);
			pData[2].tex = glm::vec2((xPos+1)/2, (yPos+1)/2);	
		
	//fill in pIndices array


			pIndices[0] = 0;
			pIndices[1] = 1;
			pIndices[2] = 2;
		
}


L3::L3(void)
{
}


L3::~L3(void)
{
}

void L3::initData()
{
	if (pData)
	{
		delete[] pData;
		delete[] pIndices;
	}


	if (!scene.LoadFile("hearse.3ds")) cout<<"bad .3ds file";

	
	if (scene.GetMeshCount()!=1) cout<< "error 3ds"; 


		LMesh &mesh = scene.GetMesh(0);
		cout<<mesh.GetTriangleCount()<<" triangles in 3ds. \r\n";





	//number of points
	dataCount = mesh.GetTriangleCount()*3; 
	//number of triangles
	unsigned int nTriangles = mesh.GetTriangleCount();
	//number of indices
	indicesCount = mesh.GetTriangleCount()*3;

	pData = new VertexData [dataCount];
	pIndices = new unsigned int [indicesCount];
	

	for (int i = 0; i< mesh.GetTriangleCount(); i++)
	{
			pData[i*3].pos = glm::vec3(mesh.GetTriangle2(i).vertices[0].x,mesh.GetTriangle2(i).vertices[0].y,mesh.GetTriangle2(i).vertices[0].z);
			pData[i*3].nor = glm::vec3(mesh.GetTriangle2(i).vertexNormals[0].x,mesh.GetTriangle2(i).vertexNormals[0].y,mesh.GetTriangle2(i).vertexNormals[0].z);
			pData[i*3].tex = glm::vec2(mesh.GetTriangle2(i).textureCoords[0].x,mesh.GetTriangle2(i).textureCoords[0].y);		
			pIndices[i*3]=i*3;

			pData[i*3+1].pos = glm::vec3(mesh.GetTriangle2(i).vertices[1].x,mesh.GetTriangle2(i).vertices[1].y,mesh.GetTriangle2(i).vertices[1].z);
			pData[i*3+1].nor = glm::vec3(mesh.GetTriangle2(i).vertexNormals[1].x,mesh.GetTriangle2(i).vertexNormals[1].y,mesh.GetTriangle2(i).vertexNormals[1].z);
			pData[i*3+1].tex = glm::vec2(mesh.GetTriangle2(i).textureCoords[1].x,mesh.GetTriangle2(i).textureCoords[1].y);		
			pIndices[i*3+1]=i*3+1;

			pData[i*3+2].pos = glm::vec3(mesh.GetTriangle2(i).vertices[2].x,mesh.GetTriangle2(i).vertices[2].y,mesh.GetTriangle2(i).vertices[2].z);
			pData[i*3+2].nor = glm::vec3(mesh.GetTriangle2(i).vertexNormals[2].x,mesh.GetTriangle2(i).vertexNormals[2].y,mesh.GetTriangle2(i).vertexNormals[2].z);
			pData[i*3+2].tex = glm::vec2(mesh.GetTriangle2(i).textureCoords[2].x,mesh.GetTriangle2(i).textureCoords[2].y);		
			pIndices[i*3+2]=i*3+2;
	}

}
