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
		a_mesh = new cMesh(world);
		a_mesh->setPos(startingPosition);
		createBoundaries(startingPosition);
		createMeshCube();
		
		world->addChild(a_mesh);
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

	boundary0 = cVector3d(-0.4+ang/60, boundary0.y, boundary0.z);
	boundary1 = cVector3d(0.4+ang/60, boundary1.y, boundary1.z);
	boundary2 = cVector3d(0.4+ang/60, boundary2.y, boundary2.z);
	boundary3 = cVector3d(-0.4+ang/60, boundary3.y, boundary3.z);

	if(isLocal){
		cVector3d prevHitPos = a_mesh->getPos();
		a_mesh->setPos(ang/60, prevHitPos.y, prevHitPos.z);
	}
}

void VirtualAvatar::updateBoundaries(double ang, cVector3d position)
{
	// Rotate the boundaries
	double opposed = cSinDeg(ang)*0.8;
	double adjacent = cCosDeg(ang)*0.8;
	double halfOpp = opposed/2;
	double halfAdj = adjacent/2;

	boundary0 = cVector3d(-halfAdj+opposed, adjacent+halfOpp, boundary0.z);
	boundary1 = cVector3d(opposed+halfAdj, adjacent-halfOpp, boundary1.z);
	boundary2 = cVector3d(halfAdj, -halfOpp, boundary2.z);
	boundary3 = cVector3d(-halfAdj, halfOpp, boundary3.z);

	//// Translate the boundaries
	//boundaryMin.x += ang/75;
	//boundaryMax.x += ang/75;
}

bool VirtualAvatar::isInHitBox(cVector3d ballPos)
{
	return ((boundary0.x < ballPos.x) && (boundary1.x > ballPos.x) &&
			(boundary3.x < ballPos.y) && (boundary0.y > ballPos.y) &&
			(boundary0.z < ballPos.z) && ((boundary0.z+0.8) > ballPos.z));
}

void VirtualAvatar::createBoundaries(cVector3d startingPosition)
{
	boundary0 = startingPosition + cVector3d(-0.4, 0.8, -0.4);
	boundary1 = startingPosition + cVector3d(0.4, 0.8, -0.4);
	boundary2 = startingPosition + cVector3d(0.4, 0.0, -0.4);
	boundary3 = startingPosition + cVector3d(-0.4, 0.0, -0.4);
}

void VirtualAvatar::createMeshCube()
{
    int vertices [6][6];

	double xMin = -0.4;
	double xMax = 0.4;
	double yMin = 0.0;
	double yMax = 0.8;
	double zMin = -0.4;
	double zMax = 0.4;

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