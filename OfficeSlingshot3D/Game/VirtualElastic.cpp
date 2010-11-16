#include "VirtualElastic.h"

VirtualElastic::VirtualElastic(cWorld* world, const cVector3d& startingPosition, bool _isLocal)
{
	isLocal = _isLocal;

	elasticMesh = new cMesh(world);
	elasticMesh->setPos(startingPosition);

	for(int i=0; i<8; i++) {
		elasticMesh->newVertex(cVector3d());
	}

	world->addChild(elasticMesh);
}

VirtualElastic::~VirtualElastic(void)
{
}

void VirtualElastic::reset()
{
	elasticMesh->setShowEnabled(false);
}

void VirtualElastic::updateElasticMesh(cVector3d slingshotUpperLeft, cVector3d slingshotUpperRight, cVector3d ballPos)
{
	// Show the elastic
	elasticMesh->setShowEnabled(true);

	float elasticWidth = 0.08f;
	float ballDepth = 0.08f;
	if (!isLocal) {
		ballDepth = -ballDepth;
	}

	// Elastic from the left fork of the slingshot to the left side of the ball
	elasticMesh->getVertex(0)->setPos(slingshotUpperLeft.x, slingshotUpperLeft.y-elasticWidth/2, slingshotUpperLeft.z);
	elasticMesh->getVertex(1)->setPos(slingshotUpperLeft.x, slingshotUpperLeft.y+elasticWidth/2, slingshotUpperLeft.z);
    elasticMesh->getVertex(2)->setPos(ballPos.x-elasticWidth, ballPos.y+elasticWidth/2, ballPos.z+ballDepth);
    elasticMesh->getVertex(3)->setPos(ballPos.x-elasticWidth, ballPos.y-elasticWidth/2, ballPos.z+ballDepth);

	// Elastic from the right fork of the slingshot to the right side of the ball
	elasticMesh->getVertex(4)->setPos(slingshotUpperRight.x, slingshotUpperRight.y-elasticWidth/2, slingshotUpperRight.z);
	elasticMesh->getVertex(5)->setPos(slingshotUpperRight.x, slingshotUpperRight.y+elasticWidth/2, slingshotUpperRight.z);
    elasticMesh->getVertex(6)->setPos(ballPos.x+elasticWidth, ballPos.y+elasticWidth/2, ballPos.z+ballDepth);
    elasticMesh->getVertex(7)->setPos(ballPos.x+elasticWidth, ballPos.y-elasticWidth/2, ballPos.z+ballDepth);

	// Left elastic
	elasticMesh->newTriangle(0, 1, 2);
	elasticMesh->newTriangle(1, 2, 3);

	// Right elastic
	elasticMesh->newTriangle(4, 5, 6);
	elasticMesh->newTriangle(5, 6, 7);

	// Middle band
	elasticMesh->newTriangle(2, 3, 6);
	elasticMesh->newTriangle(3, 6, 7);

	cMaterial mat0;
    mat0.m_ambient.set(0.0, 0.0, 0.0);
    mat0.m_diffuse.set(0.97f, 0.91f, 0.49f);
    elasticMesh->setMaterial(mat0);

	// Render both sides of the mesh object
	elasticMesh->setUseCulling(false);
}
