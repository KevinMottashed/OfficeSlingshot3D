#include "VirtualEnvironment.h"

VirtualEnvironment::VirtualEnvironment(void)
{
	world = new cWorld();
	ODEWorld = new cODEWorld(world);
	camera = new cCamera(world);
	light = new cLight(world);
	slingshot = new cMesh(world);
	avatar = new cMesh(world);
	ball = new cMesh(world);
	ground = new cMesh(world);
}

VirtualEnvironment::~VirtualEnvironment(void)
{
	delete world;
	delete camera;
	delete light;
	delete slingshot;
	delete avatar;
	delete ball;
	delete ground;
}

cCamera* VirtualEnvironment::getCamera(void)
{
	return camera;
}

cVector3d VirtualEnvironment::updateFrame(void)
{
	ODEWorld->updateDynamics(0.02);

	camera->set( ODEBall->getPos(),    // camera position (eye)
        ODEBall->getPos(),    // lookat position (target)
        cVector3d (0.0f, 0.0f, 1.0f));   // direction of the "up" vector

	return ODEBall->getPos();
}

void VirtualEnvironment::shootBall(void)
{
	ODEBall->setPosition(cVector3d(5.0f, 0.0f, -0.2f));

	//Add a force just for show
	ODEBall->addGlobalForceAtGlobalPos(cVector3d(-500.0f, 0.0f, 300.0f),ODEBall->getPos());
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

    // add ODE world as a node inside world
    world->addChild(ODEWorld);

    // set some gravity
	ODEWorld->setGravity(cVector3d(0.0, 0.0, -9.81));


	//**************************************//
	//               CAMERA                 //
	//**************************************//

    // position and oriente the camera
    camera->set( cVector3d (7.0f, 0.0f, 0.3f),    // camera position (eye)
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
	//              SLINGSHOT               //
	//**************************************//

    // add object to world
    world->addChild(slingshot);

    slingshot->setPos(5.0f, 0.0f, -1.4f);

	slingshot->rotate( cVector3d(0, 1, 0), cDegToRad(90));
	slingshot->rotate( cVector3d(1, 0, 0), cDegToRad(90));

	slingshot->loadFromFile("Objects\\slingshot\\slingshot.obj");
	slingshot->scale(3);

    // compute a boundary box
    slingshot->computeBoundaryBox(true);

    // define some haptic friction properties
    slingshot->setFriction(0.1f, 0.2f, true);

	slingshot->setUseCulling(false, true);

	//**************************************//
	//                AVATAR                //
	//**************************************//

	avatar->rotate( cVector3d(0, 1, 0), cDegToRad(90));
	avatar->rotate( cVector3d(1, 0, 0), cDegToRad(90));

	avatar->loadFromFile("Objects\\avatar\\avatar.obj");
	avatar->scale(0.5f);

	// compute collision detection algorithm
    avatar->createAABBCollisionDetector(1.01, true, true);

	avatar->setUseCulling(false, false);

	//**************************************//
	//                 BALL                 //
	//**************************************//

	ball->loadFromFile("Objects\\ball\\ball.obj");
	ball->scale(0.02f);

	// compute collision detection algorithm
    ball->createAABBCollisionDetector(1.01, true, true);

	ball->setUseCulling(false, false);

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

    // enable and set transparency level of ground
    ground->setTransparencyLevel(0.75f);
    ground->setUseTransparency(true);

	//**************************************//
	//           PHYSICS OBJECTS            //
	//**************************************//

	//Create physic ball
	ODEBall = new cODEGenericBody(ODEWorld);
    ODEBall->setImageModel(ball);

	//Calculate boundaries of physical ball
	ODEBall->createDynamicMesh(false);

	ODEBall->setPosition(cVector3d(5.0f, 0.0f, -0.2f));
	
	//Create physic avatar
	ODEAvatar = new cODEGenericBody(ODEWorld);
    ODEAvatar->setImageModel(avatar);

	//Calculate boundaries of physical avatar
	ODEAvatar->createDynamicBoundingBox(true);

	ODEAvatar->setPos(cVector3d(-5.0f, 0.0f, -1.0f));

	avatar->getBoundaryMax();
	avatar->getBoundaryMin();

	//Create a static ground plane
    ODEGround = new cODEGenericBody(ODEWorld);
    ODEGround->createStaticPlane(cVector3d(0.0f, 0.0f, -1.0f), cVector3d(0.0f, 0.0f, 1.0f));
}