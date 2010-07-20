#pragma once

#include "chai3d.h"
#include "CODE.h"

#include <iostream>
#include <fstream>
using namespace std;

class VirtualEnvironment
{
private:
	cCamera* camera;
	cWorld* world;
	cLight* light;
	cMesh* slingshot;
	cMesh* avatar;
	cMesh* ball;
	cMesh* ground;
	cGenericObject* reflexion;

	cODEWorld* ODEWorld;
	cODEGenericBody* ODEBall;
	cODEGenericBody* ODEAvatar;
	cODEGenericBody* ODEGround;

public:
	VirtualEnvironment(void);
	~VirtualEnvironment(void);

	void initialize(void);
	void createRectangle(cMesh* a_mesh, double width, double height, double depth);
	cCamera* getCamera(void);
	cVector3d updateFrame(void);
	void shootBall(void);
};
