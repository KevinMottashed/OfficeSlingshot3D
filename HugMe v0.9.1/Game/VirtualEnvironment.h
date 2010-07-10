#pragma once

#include "chai3d.h"
#include "CODE.h"

class VirtualEnvironment
{
private:
	cCamera* camera;
	cWorld* world;
	cLight* light;
	cMesh* slinghot;
	cMesh* avatar;
	cMesh* ball;
	cMesh* ground;
	cGenericObject* reflexion;

	cODEWorld* ODEWorld;
	cODEGenericBody* ODEBall;
	cODEGenericBody* ODEGround;

public:
	VirtualEnvironment(void);
	~VirtualEnvironment(void);

	void initialize(void);
	void createRectangle(cMesh* a_mesh, double width, double height, double depth);
	cCamera* getCamera(void);
	void updateFrame(void);
};
