#include "VirtualAvatar.h"

VirtualAvatar::VirtualAvatar(cWorld* world, cVector3d startingPosition, bool isLocal): 
	isLocal(isLocal)
{
	avatarMesh = new cMesh(world);

	world->addChild(avatarMesh);

	avatarMesh->loadFromFile("Objects\\avatar\\avatar.obj");
	avatarMesh->scale(0.045f);

	avatarMesh->setPos(startingPosition);

	if(!isLocal){
		avatarMesh->rotate(cVector3d(0.0f, 1.0f, 0.0f), cDegToRad(-90));
	} else {
		avatarMesh->rotate(cVector3d(0.0f, 1.0f, 0.0f), cDegToRad(90));
	}

	avatarMesh->setUseCulling(true, true);

	if(isLocal){
		createBoundaries(startingPosition);

		chestHitBox = new cMesh(world);
		rArmHitBox = new cMesh(world);
		lArmHitBox = new cMesh(world);

		chestHitBox->setPos(startingPosition);
		createMeshCube(chestHitBox, chestMin, chestMax);
		
		rArmHitBox->setPos(startingPosition);
		createMeshCube(rArmHitBox, rArmMin, rArmMax);

		lArmHitBox->setPos(startingPosition);
		createMeshCube(lArmHitBox, lArmMin, lArmMax);
		
		world->addChild(chestHitBox);
		world->addChild(rArmHitBox);
		world->addChild(lArmHitBox);
	}
}

VirtualAvatar::~VirtualAvatar(void)
{
}

void VirtualAvatar::rotate(double ang)
{
	//Reset rotation to upright
	avatarMesh->rotate(avatarMesh->getRot().inv());

	if(!isLocal){
		ang = -ang;
		avatarMesh->rotate(cVector3d(0.0f, 1.0f, 0.0f), cDegToRad(-90));
	} else {
		avatarMesh->rotate(cVector3d(0.0f, 1.0f, 0.0f), cDegToRad(90));
	}

	//Apply rotation
	avatarMesh->rotate(cVector3d(0.0f, 0.0f, 1.0f), cDegToRad(-ang));
}

void VirtualAvatar::translate(double ang)
{
	if(!isLocal) ang = -ang;

	cVector3d prevPos = avatarMesh->getPos();
	avatarMesh->setPos(ang/75, prevPos.y, prevPos.z);

	chestMin = cVector3d(-0.3+ang/50, chestMin.y, chestMin.z);
	chestMax = cVector3d(0.3+ang/50, chestMax.y, chestMax.z);

	rArmMin = cVector3d(0.3+ang/50, rArmMin.y, rArmMin.z);
	rArmMax = cVector3d(0.5+ang/50, rArmMax.y, rArmMax.z);

	lArmMin = cVector3d(-0.5+ang/50, lArmMin.y, lArmMin.z);
	lArmMax = cVector3d(-0.3+ang/50, lArmMax.y, lArmMax.z);

	if(isLocal){
		cVector3d prevChestPos = chestHitBox->getPos();
		chestHitBox->setPos(ang/50, prevChestPos.y, prevChestPos.z);

		cVector3d prevRArmPos = rArmHitBox->getPos();
		rArmHitBox->setPos(ang/50, prevRArmPos.y, prevRArmPos.z);

		cVector3d prevLArmPos = lArmHitBox->getPos();
		lArmHitBox->setPos(ang/50, prevLArmPos.y, prevLArmPos.z);
	}
}

void VirtualAvatar::updateBoundaries(double ang, cVector3d position)
{
	// Rotate the boundaries
	double opposed = cSinDeg(ang)*0.8;
	double adjacent = cCosDeg(ang)*0.8;
	double halfOpp = opposed/2;
	double halfAdj = adjacent/2;

	/*
	boundary0 = cVector3d(-halfAdj+opposed, adjacent+halfOpp, boundary0.z);
	boundary1 = cVector3d(opposed+halfAdj, adjacent-halfOpp, boundary1.z);
	boundary2 = cVector3d(halfAdj, -halfOpp, boundary2.z);
	boundary3 = cVector3d(-halfAdj, halfOpp, boundary3.z);
	*/

	//// Translate the boundaries
	//boundaryMin.x += ang/75;
	//boundaryMax.x += ang/75;
}

bool VirtualAvatar::isInHitBox(cVector3d ballPos)
{
	if (((chestMin.x < ballPos.x) && (chestMax.x > ballPos.x) &&
			(chestMin.y < ballPos.y) && (chestMax.y > ballPos.y) &&
			(chestMin.z < ballPos.z) && (chestMax.z > ballPos.z))) {
		// CHEST = 0
		hitPart = 0;
		return true;
	} else if (((rArmMin.x < ballPos.x) && (rArmMax.x > ballPos.x) &&
			(rArmMin.y < ballPos.y) && (rArmMax.y > ballPos.y) &&
			(rArmMin.z < ballPos.z) && (rArmMax.z > ballPos.z))) {
		hitPart = 1;
		return true;
	} else if (((lArmMin.x < ballPos.x) && (lArmMax.x > ballPos.x) &&
			(lArmMin.y < ballPos.y) && (lArmMax.y > ballPos.y) &&
			(lArmMin.z < ballPos.z) && (lArmMax.z > ballPos.z))) {
		hitPart = 3;
		return true;
	}
	return false;
}

void VirtualAvatar::createBoundaries(cVector3d startingPosition)
{
	chestMax = startingPosition + cVector3d(0.3, 0.8, 0.4);
	chestMin = startingPosition + cVector3d(-0.3, 0.0, -0.4);

	rArmMax = startingPosition + cVector3d(0.5, 0.8, 0.4);
	rArmMin = startingPosition + cVector3d(0.3, 0.0, -0.4);

	lArmMax = startingPosition + cVector3d(-0.3, 0.8, 0.4);
	lArmMin = startingPosition + cVector3d(-0.5, 0.0, -0.4);
}

int VirtualAvatar::getHitBodyPart()
{
	return hitPart;
}

cVector3d VirtualAvatar::getChestMin()
{
	return chestMin;
}

cVector3d VirtualAvatar::getChestMax()
{
	return chestMax;
}

cVector3d VirtualAvatar::getRightArmMin()
{
	return rArmMin;
}

cVector3d VirtualAvatar::getRightArmMax()
{
	return rArmMax;
}

cVector3d VirtualAvatar::getLeftArmMin()
{
	return lArmMin;
}

cVector3d VirtualAvatar::getLeftArmMax()
{
	return lArmMax;
}

void VirtualAvatar::createMeshCube(cMesh* a_mesh, cVector3d minVector, cVector3d maxVector)
{
    int vertices [6][6];

	double xMin = minVector.x;
	double xMax = maxVector.x;
	double yMin = minVector.y;
	double yMax = maxVector.y;
	double zMin = minVector.z - 6.0;
	double zMax = maxVector.z - 6.0;

    // face -x
    vertices[0][0] = a_mesh->newVertex(xMin, yMax, zMin);
    vertices[0][1] = a_mesh->newVertex(xMin, yMin, zMin);
    vertices[0][2] = a_mesh->newVertex(xMin, yMin, zMax);
    vertices[0][3] = a_mesh->newVertex(xMin, yMax, zMax);

    // face +x
    vertices[1][0] = a_mesh->newVertex( xMax, yMin, zMin);
    vertices[1][1] = a_mesh->newVertex( xMax, yMax, zMin);
    vertices[1][2] = a_mesh->newVertex( xMax, yMax, zMax);
    vertices[1][3] = a_mesh->newVertex( xMax, yMin, zMax);

    // face -y
    vertices[2][0] = a_mesh->newVertex( xMin, yMin, zMin);
    vertices[2][1] = a_mesh->newVertex( xMax, yMin, zMin);
    vertices[2][2] = a_mesh->newVertex( xMax, yMin, zMax);
    vertices[2][3] = a_mesh->newVertex( xMin, yMin, zMax);

    // face +y
    vertices[3][0] = a_mesh->newVertex( xMax, yMax, zMin);
    vertices[3][1] = a_mesh->newVertex( xMin, yMax, zMin);
    vertices[3][2] = a_mesh->newVertex( xMin, yMax, zMax);
    vertices[3][3] = a_mesh->newVertex( xMax, yMax, zMax);

    // face -z
    vertices[4][0] = a_mesh->newVertex( xMin, yMin, zMin);
    vertices[4][1] = a_mesh->newVertex( xMin, yMax, zMin);
    vertices[4][2] = a_mesh->newVertex( xMax, yMax, zMin);
    vertices[4][3] = a_mesh->newVertex( xMax, yMin, zMin);

    // face +z
    vertices[5][0] = a_mesh->newVertex( xMax, yMin,  zMax);
    vertices[5][1] = a_mesh->newVertex( xMax, yMax,  zMax);
    vertices[5][2] = a_mesh->newVertex( xMin, yMax,  zMax);
    vertices[5][3] = a_mesh->newVertex( xMin, yMin,  zMax);

    // create triangles
    for (int i=0; i<6; i++)
    {
		a_mesh->newTriangle(vertices[i][0], vertices[i][1], vertices[i][2]);
		a_mesh->newTriangle(vertices[i][0], vertices[i][2], vertices[i][3]);
	}
	a_mesh->setWireMode(true);
}