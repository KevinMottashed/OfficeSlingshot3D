#pragma once

#include "chai3d.h"

class VirtualEnvironment
{
private:
	cCamera* camera;
	cWorld* world;
	cLight* light;
	cMesh* slinghot;
	cMesh* avatar;
	cMesh* ground;
	cGenericObject* reflexion;

public:
	VirtualEnvironment(void);
	~VirtualEnvironment(void);

	void initialize(void);
	void createRectangle(cMesh* a_mesh, double width, double height, double depth);
	cCamera* getCamera(void);
};
