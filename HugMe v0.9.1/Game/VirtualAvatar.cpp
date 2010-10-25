#include "VirtualAvatar.h"

const cVector3d VirtualAvatar::iniChestMax = cVector3d(0.3, 0.8, 0.4);
const cVector3d VirtualAvatar::iniChestMin = cVector3d(-0.3, 0.0, -0.4);

const cVector3d VirtualAvatar::iniLeftArmMax = cVector3d(-0.3, 0.8, 0.4);
const cVector3d VirtualAvatar::iniLeftArmMin = cVector3d(-0.5, 0.0, -0.4);

const cVector3d VirtualAvatar::iniRightArmMax = cVector3d(0.5, 0.8, 0.4);
const cVector3d VirtualAvatar::iniRightArmMin = cVector3d(0.3, 0.0, -0.4);

const cVector3d VirtualAvatar::iniHeadMax = cVector3d(0.15, 1.3, 0.4);
const cVector3d VirtualAvatar::iniHeadMin = cVector3d(-0.15, 0.8, -0.4);

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
		headHitBox = new cMesh(world);

		chestHitBox->setPos(startingPosition);
		createMeshCube(chestHitBox, chestMin, chestMax);
		
		rArmHitBox->setPos(startingPosition);
		createMeshCube(rArmHitBox, rArmMin, rArmMax);

		lArmHitBox->setPos(startingPosition);
		createMeshCube(lArmHitBox, lArmMin, lArmMax);

		headHitBox->setPos(startingPosition);
		createMeshCube(headHitBox, headMin, headMax);
		
		world->addChild(chestHitBox);
		world->addChild(rArmHitBox);
		world->addChild(lArmHitBox);
		world->addChild(headHitBox);
	}
}

VirtualAvatar::~VirtualAvatar(void)
{
}

void VirtualAvatar::rotate(double ang)
{
	double xTrans = (cSinDeg(ang)*0.8)*2;
	double yTrans = 0.8-(cCosDeg(ang)*0.8);

	translate(xTrans, yTrans);

	/*
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
	*/
}

void VirtualAvatar::translate(double xTrans, double yTrans)
{
	cVector3d prevPos = avatarMesh->getPos();
	avatarMesh->setPos(xTrans, -yTrans, prevPos.z);

	chestMin = cVector3d(iniChestMin.x+xTrans, iniChestMin.y-yTrans, chestMin.z);
	chestMax = cVector3d(iniChestMax.x+xTrans, iniChestMax.y-yTrans, chestMax.z);

	rArmMin = cVector3d(iniRightArmMin.x+xTrans, iniRightArmMin.y-yTrans, rArmMin.z);
	rArmMax = cVector3d(iniRightArmMax.x+xTrans, iniRightArmMax.y-yTrans, rArmMax.z);

	lArmMin = cVector3d(iniLeftArmMin.x+xTrans, iniLeftArmMin.y-yTrans, lArmMin.z);
	lArmMax = cVector3d(iniLeftArmMax.x+xTrans, iniLeftArmMax.y-yTrans, lArmMax.z);

	headMin = cVector3d(iniHeadMin.x+xTrans, iniHeadMin.y-yTrans, iniHeadMin.z);
	headMax = cVector3d(iniHeadMax.x+xTrans, iniHeadMax.y-yTrans, iniHeadMax.z);

	if(isLocal){
		cVector3d prevChestPos = chestHitBox->getPos();
		chestHitBox->setPos(xTrans, -yTrans, prevChestPos.z);

		cVector3d prevRArmPos = rArmHitBox->getPos();
		rArmHitBox->setPos(xTrans, -yTrans, prevRArmPos.z);

		cVector3d prevLArmPos = lArmHitBox->getPos();
		lArmHitBox->setPos(xTrans, -yTrans, prevLArmPos.z);

		cVector3d prevHeadPos = headHitBox->getPos();
		headHitBox->setPos(xTrans, -yTrans, prevHeadPos.z);
	}
}

// TODO: Remove if rotation is not needed !!
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
		// HumanPart::CHEST = 0
		hitPart = 0;
		return true;
	} else if (((rArmMin.x < ballPos.x) && (rArmMax.x > ballPos.x) &&
			(rArmMin.y < ballPos.y) && (rArmMax.y > ballPos.y) &&
			(rArmMin.z < ballPos.z) && (rArmMax.z > ballPos.z))) {
		// HumanPart::LEFT_UPPER_ARM = 1
		hitPart = 1;
		return true;
	} else if (((lArmMin.x < ballPos.x) && (lArmMax.x > ballPos.x) &&
			(lArmMin.y < ballPos.y) && (lArmMax.y > ballPos.y) &&
			(lArmMin.z < ballPos.z) && (lArmMax.z > ballPos.z))) {
		// HumanPart::RIGHT_UPPER_ARM = 3
		hitPart = 3;
		return true;
	} else if (((headMin.x < ballPos.x) && (headMax.x > ballPos.x) &&
			(headMin.y < ballPos.y) && (headMax.y > ballPos.y) &&
			(headMin.z < ballPos.z) && (headMax.z > ballPos.z))) {
		// HumanPart::HEAD = 5
		hitPart = 5;
		return true;
	}
	return false;
}

void VirtualAvatar::createBoundaries(cVector3d startingPosition)
{
	chestMax = startingPosition + iniChestMax;
	chestMin = startingPosition + iniChestMin;

	rArmMax = startingPosition + iniRightArmMax;
	rArmMin = startingPosition + iniRightArmMin;

	lArmMax = startingPosition + iniLeftArmMax;
	lArmMin = startingPosition + iniLeftArmMin;

	headMax = startingPosition + iniHeadMax;
	headMin = startingPosition + iniHeadMin;
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

cVector3d VirtualAvatar::getHeadMin()
{
	return headMin;
}

cVector3d VirtualAvatar::getHeadMax()
{
	return headMax;
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