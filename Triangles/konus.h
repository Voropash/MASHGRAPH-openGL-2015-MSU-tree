#pragma once
#include "MyObject.h"
#include "l3ds.h" // Header file for the L3ds Library
//#include "l3ds.cpp" // Cpp file for the L3ds Library

//branch of tree is cylinder
class Konus :
	public MyObject
{
public:
	Konus(void);
	virtual ~Konus(void);
	//redefinition of 
	virtual void initData();
};

class L3 :
	public MyObject
{
public:
	L3DS scene;
	L3(void);
	virtual ~L3(void);
	//redefinition of 
	virtual void initData();
};

