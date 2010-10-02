#ifndef VIRTUAL_ENVIRONMENT_H
#define VIRTUAL_ENVIRONMENT_H

#include "chai3d.h"
#include "CODE.h"

class VirtualEnvironment
{
public:
	VirtualEnvironment(void);
	~VirtualEnvironment(void);

	/**
	 * Get the environments camera
	 * @return the camera
	 */
	cCamera* camera();

	/**
	 * Move the local slingshot to a new position
	 * @param position The new position
	 */
	void moveLocalSlingshot(cVector3d position);

	/**
	 * Move the peer slingshot to a new position
	 * @param position The new position
	 */
	void movePeerSlingshot(cVector3d position);

	void initialize(void);
	void createRectangle(cMesh* a_mesh, double width, double height, double depth);
	void updateFrame();
	void createNewBall(int numBalls);
	void shootBall(cVector3d force);
	void receiveBall(cVector3d force);

private:
	cCamera* _camera;
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

};
#endif