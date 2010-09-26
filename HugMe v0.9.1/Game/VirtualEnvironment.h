#ifndef VIRTUAL_ENVIRONMENT_H
#define VIRTUAL_ENVIRONMENT_H

#include "chai3d.h"
#include "CODE.h"

class VirtualEnvironment
{
private:
	cCamera* camera;
	cWorld* world;
	cLight* light;
	cMesh* rSlingshot;
	cMesh* lSlingshot;
	cMesh* rAvatar;
	cMesh* lAvatar;
	cMesh* ball;
	cMesh* ground;

	cODEWorld* ODEWorld;
	cODEGenericBody* ODEBall[4];
	cODEGenericBody* rODEAvatar;
	cODEGenericBody* lODEAvatar;
	cODEGenericBody* ODEGround;

	int rNumBalls;
	int lNumBalls;

public:
	VirtualEnvironment(void);
	~VirtualEnvironment(void);

	void initialize(void);
	void createRectangle(cMesh* a_mesh, double width, double height, double depth);
	void updateFrame(int displayW, int displayH);
	void createNewBall(int numBalls);
	void shootBall(cVector3d force);
	void receiveBall(cVector3d force);
};
#endif