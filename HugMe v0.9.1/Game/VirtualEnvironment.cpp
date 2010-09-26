#include "VirtualEnvironment.h"

VirtualEnvironment::VirtualEnvironment(void)
{
	world = new cWorld();
	camera = new cCamera(world);
	light = new cLight(world);
	rSlingshot = new cMesh(world);
	lSlingshot = new cMesh(world);
	rAvatar = new cMesh(world);
	lAvatar = new cMesh(world);
	ball = new cMesh(world);
	ground = new cMesh(world);
}

VirtualEnvironment::~VirtualEnvironment(void)
{
	// we only need to delete world because it is the parent object
	// and will delete all its children when it is deleted
	delete world;
}

void VirtualEnvironment::updateFrame(int displayW, int displayH)
{
	camera->renderView(displayW, displayH);

	ODEWorld->updateDynamics(0.02);

	ODEWorld->computeGlobalPositions(true);
}

void VirtualEnvironment::createNewBall(int numBalls)
{
	ODEBall[numBalls]->setImageModel(ball);

	//Calculate boundaries of physical ball
	ODEBall[numBalls]->createDynamicMesh(false);

	ODEBall[numBalls]->setUseCulling(false, true);
}

void VirtualEnvironment::receiveBall(cVector3d force)
{
	createNewBall((rNumBalls%2)+2);

	ODEBall[(rNumBalls%2)+2]->setPosition(cVector3d(-5.0f, 0.0f, -0.2f));

	//Add a force just for show
	ODEBall[(rNumBalls%2)+2]->addGlobalForceAtGlobalPos(force,ODEBall[(rNumBalls%2)+2]->getPos());
	
	rNumBalls++;
}

void VirtualEnvironment::shootBall(cVector3d force)
{
	createNewBall(lNumBalls%2);

	ODEBall[lNumBalls%2]->setPosition(cVector3d(5.0f, 0.0f, -0.2f));

	//Add a force just for show
	ODEBall[lNumBalls%2]->addGlobalForceAtGlobalPos(force,ODEBall[lNumBalls%2]->getPos());

	lNumBalls++;
}

void VirtualEnvironment::initialize(void)
{
	//**************************************//
	//                WORLD                 // 
	//**************************************//

	world->addChild(camera);

    // set the background color of the environment
    // the color is defined by its (R,G,B) components.
    world->setBackgroundColor(0.5f, 0.37f, 0.28f);

	//**************************************//
	//               CAMERA                 //
	//**************************************//

    // position and oriente the camera
    camera->set( cVector3d (7.0f, 0.0f, 0.0f),    // camera position (eye)
        cVector3d (0.0f, 0.0f, 0.0f),    // lookat position (target)
        cVector3d (0.0f, 0.0f, 1.0f));   // direction of the "up" vector

    // set the near and far clipping planes of the camera
    // anything in front/behind these clipping planes will not be rendered
    camera->setClippingPlanes(0.0f, 7.0f);

    // enable high quality rendering
    camera->enableMultipassTransparency(true);

	//**************************************//
	//                LIGHT                 //
	//**************************************//

    // create a light source and attach it to the camera
    camera->addChild(light);                   // attach light to camera
    light->setEnabled(true);                   // enable light source
    light->setPos(cVector3d( 2.0f, 0.5f, 1.0f));  // position the light source
    light->setDir(cVector3d(-2.0f, 0.5f, 1.0f));  // define the direction of the light beam
    light->m_ambient.set(0.6f, 0.6f, 0.6f);
    light->m_diffuse.set(0.8f, 0.8f, 0.8f);
    light->m_specular.set(0.8f, 0.8f, 0.8f);

	//**************************************//
	//              ODE WORLD               //
	//**************************************//

	ODEWorld = new cODEWorld(world);

	// add ODE world as a node inside world
    world->addChild(ODEWorld);

    // set some gravity
	ODEWorld->setGravity(cVector3d(0.0, 0.0, -9.81));

	//**************************************//
	//              SLINGSHOT               //
	//**************************************//

    // add object to world
    world->addChild(rSlingshot);
	world->addChild(lSlingshot);

    rSlingshot->setPos(-5.0f, 0.0f, -1.2f);
	lSlingshot->setPos(5.0f, 0.0f, -1.2f);

	rSlingshot->rotate( cVector3d(0, 1, 0), cDegToRad(90));
	rSlingshot->rotate( cVector3d(1, 0, 0), cDegToRad(90));
	lSlingshot->rotate( cVector3d(0, 1, 0), cDegToRad(90));
	lSlingshot->rotate( cVector3d(1, 0, 0), cDegToRad(90));

	rSlingshot->loadFromFile("Objects\\slingshot\\slingshot.obj");
	rSlingshot->scale(2);
	lSlingshot->loadFromFile("Objects\\slingshot\\slingshot.obj");
	lSlingshot->scale(2);

    // compute a boundary box
    rSlingshot->computeBoundaryBox(true);
	lSlingshot->computeBoundaryBox(true);

    // define some haptic friction properties
    rSlingshot->setFriction(0.1f, 0.2f, true);
	lSlingshot->setFriction(0.1f, 0.2f, true);

	rSlingshot->setUseCulling(false, true);
	lSlingshot->setUseCulling(false, true);

	//**************************************//
	//                AVATARS               //
	//**************************************//

	lAvatar->loadFromFile("Objects\\avatar\\avatar.obj");
	lAvatar->scale(0.045f);

	lAvatar->createAABBCollisionDetector(1.01, true, false);

	lAvatar->setTransparencyLevel(0.25, true, true);

	rAvatar->loadFromFile("Objects\\avatar\\avatar.obj");
	rAvatar->scale(0.045f);

	rAvatar->createAABBCollisionDetector(1.01, true, false);

	rODEAvatar = new cODEGenericBody(ODEWorld);
	lODEAvatar = new cODEGenericBody(ODEWorld);

    rODEAvatar->setImageModel(rAvatar);
	lODEAvatar->setImageModel(lAvatar);

	//Calculate boundaries of physical avatar
	rODEAvatar->createDynamicMesh(true);
	lODEAvatar->createDynamicMesh(true);

	rODEAvatar->rotate( cVector3d(1, 0, 0), cDegToRad(90));
	lODEAvatar->rotate( cVector3d(1, 0, 0), cDegToRad(90));

	rODEAvatar->setPosition(cVector3d(-6.0f, 0.0f, -1.0f));
	lODEAvatar->setPosition(cVector3d(6.0f, 0.0f, -1.0f));

	rODEAvatar->setUseCulling(true, true);
	lODEAvatar->setUseCulling(true, true);

	//**************************************//
	//                 BALL                 //
	//**************************************//

	ODEBall[0] = new cODEGenericBody(ODEWorld);
	ODEBall[1] = new cODEGenericBody(ODEWorld);
	ODEBall[2] = new cODEGenericBody(ODEWorld);
	ODEBall[3] = new cODEGenericBody(ODEWorld);

	ball->loadFromFile("Objects\\ball\\ball.obj");
	ball->scale(0.015f);

	ball->createAABBCollisionDetector(1.01, true, false);

	//**************************************//
	//               GROUND                 //
	//**************************************//

    world->addChild(ground);

    // create 4 vertices (one at each corner)
    double groundSize = 10.0;
    int vertices0 = ground->newVertex(-groundSize, -groundSize, 0.0f);
    int vertices1 = ground->newVertex( groundSize, -groundSize, 0.0f);
    int vertices2 = ground->newVertex( groundSize,  groundSize, 0.0f);
    int vertices3 = ground->newVertex(-groundSize,  groundSize, 0.0f);

    // compose surface with 2 triangles
    ground->newTriangle(vertices0, vertices1, vertices2);
    ground->newTriangle(vertices0, vertices2, vertices3);

    // compute surface normals
    ground->computeAllNormals();

    // position ground at the right level
    ground->setPos(0.0f, 0.0f, -1.0f);

    // define some material properties and apply to mesh
    cMaterial matGround;
    matGround.setDynamicFriction(0.7f);
    matGround.setStaticFriction(1.0f);
    matGround.m_ambient.set(1.0f, 1.0f, 1.0f);
    matGround.m_diffuse.set(1.0f, 1.0f,1.0f);
    matGround.m_specular.set(1.0f, 1.0f, 1.0f);
    ground->setMaterial(matGround);

	//Create a static ground plane
    ODEGround = new cODEGenericBody(ODEWorld);
    ODEGround->createStaticPlane(cVector3d(0.0f, 0.0f, -1.0f), cVector3d(0.0f, 0.0f, 1.0f));

	rNumBalls = 0;
	lNumBalls = 0;
}