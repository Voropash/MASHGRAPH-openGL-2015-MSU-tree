#pragma once
#include "MyObject.h"

//branch of tree is flat object, symmetric around y axes
//example equation: y=+-sqrt(1 - x*x/100) (also is symmetric around x)
class Leaf :
	public MyObject
{
public:
	Leaf(void);
	virtual ~Leaf(void);
	virtual void initData();

private:
	float equation(float x);
};


class Leaf2 :
	public MyObject
{
public:
	Leaf2(void);
	virtual ~Leaf2(void);
	virtual void initData();

private:
	float equation(float x);
};