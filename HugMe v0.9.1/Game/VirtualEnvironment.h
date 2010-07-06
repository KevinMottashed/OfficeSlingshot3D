#pragma once

#include "chai3d.h"

class VirtualEnvironment
{
public:
	VirtualEnvironment(void);
	~VirtualEnvironment(void);

	void initialize(cWorld* world, cCamera* camera);
	void createRectangle(cMesh* a_mesh, double width, double height, double depth);
};
