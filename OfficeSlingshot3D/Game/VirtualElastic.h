#pragma once

#include "chai3d.h"

class VirtualElastic
{
public:
	/**
	 * Constructor
	 * @param world The chai world
	 * @param startingPosition The starting position within the world
	 * @param isLocal True if the elastic is local
	 */
	VirtualElastic(cWorld* world, const cVector3d& startingPosition, bool isLocal);

	/**
	 * Destructor
	 */
	~VirtualElastic(void);

	/**
	 * Updates the mesh object depending on the ball and slingshot positions
	 * It moves the mesh vertexes depending on the 2 positions
	 * @param slingshotUpperLeft The position of the left fork of the slingshot
	 * @param slingshotUpperRight The position of the right fork of the slingshot
	 * @param ballPos The position of the ball
	 */
	void updateElasticMesh(cVector3d slingshotUpperLeft, cVector3d slingshotUpperRight, cVector3d ballPos);

	/**
	 * Hides the elastics after the ball has been shot
	 */
	void hide();

private:
	/**
	 * Mesh object for the elastic
	 */
	cMesh* elasticMesh;

	/**
	 * True if the elastic is local
	 */
	bool isLocal;
};
