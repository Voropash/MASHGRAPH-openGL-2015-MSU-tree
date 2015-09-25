#pragma once
#include "MyObject.h"


class Sun :
	public MyObject
{
public:
	Sun(void);
	virtual ~Sun(void);
	virtual void initData();
};
