#include "VirtualEnvironment.h"

VirtualEnvironment::VirtualEnvironment(void)
{
	world = new cWorld();
	camera = new cCamera(world);
	light = new cLight(world);
	slinghot = new cMesh(world);
	avatar = new cMesh(world);
	ball = new cMesh(world);
	ground = new cMesh(world);
	reflexion = new cGenericObject();
}

VirtualEnvironment::~VirtualEnvironment(void)
{
}

cCamera* VirtualEnvironment::getCamera(void)
{
	return camera;
}

void VirtualEnvironment::moveBall(void)
{
	ball->translate( cVector3d(-0.01f, 0.0f, 0.0f) );
}

void VirtualEnvironment::initialize(void){

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
    camera->set( cVector3d (7.0f, 0.0f, 0.3f),    // camera position (eye)
        cVector3d (0.0f, 0.0f, 0.0f),    // lookat position (target)
        cVector3d (0.0f, 0.0f, 1.0f));   // direction of the "up" vector

    // set the near and far clipping planes of the camera
    // anything in front/behind these clipping planes will not be rendered
    camera->setClippingPlanes(0.0f, 1000.0f);

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
    world->addChild(slinghot);

    slinghot->setPos(5.0f, 0.0f, -1.4f);

	slinghot->rotate( cVector3d(0, 1, 0), cDegToRad(90));
	slinghot->rotate( cVector3d(1, 0, 0), cDegToRad(90));

	slinghot->loadFromFile("Objects\\slingshot\\slingshot.obj");
	slinghot->scale(3);

    // compute a boundary box
    slinghot->computeBoundaryBox(true);

    // define some haptic friction properties
    slinghot->setFriction(0.1f, 0.2f, true);

	slinghot->setUseCulling(false, true);

	//**************************************//
	//                AVATAR                //
	//**************************************//

    // add object to world
    world->addChild(avatar);

    avatar->setPos(-3.0f, 0.0f, -1.0f);

	avatar->rotate( cVector3d(0, 1, 0), cDegToRad(90));
	avatar->rotate( cVector3d(1, 0, 0), cDegToRad(90));

	avatar->loadFromFile("Objects\\avatar\\avatar.obj");
	avatar->scale(0.5f);

    // compute a boundary box
    avatar->computeBoundaryBox(true);
	avatar->setShowBox(true);

    // define some haptic friction properties
    //avatar->setFriction(0.1, 0.2, true);

	avatar->setUseCulling(false, true);

	//**************************************//
	//                 BALL                 //
	//**************************************//

    // add object to world
    world->addChild(ball);

    ball->setPos(0.0f, 0.5f, 1.0f);

	ball->rotate( cVector3d(0, 1, 0), cDegToRad(90));
	ball->rotate( cVector3d(1, 0, 0), cDegToRad(90));

	ball->loadFromFile("Objects\\ball\\ball.obj");
	ball->scale(0.05f);

    // compute a boundary box
    ball->computeBoundaryBox(true);
	ball->setShowBox(true);

    // define some haptic friction properties
    //ball->setFriction(0.1, 0.2, true);

	ball->setUseCulling(false, true);

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
    matGround.m_ambient.set(0.0f, 0.0f, 0.0f);
    matGround.m_diffuse.set(0.0f, 0.0f, 0.0f);
    matGround.m_specular.set(0.0f, 0.0f, 0.0f);
    ground->setMaterial(matGround);

    // enable and set transparency level of ground
    ground->setTransparencyLevel(0.75f);
    ground->setUseTransparency(true);

	//**************************************//
	//              REFLEXION               //
	//**************************************//

    // set this object as a ghost node so that no haptic interactions or
    // collision detecting take place within the child nodes added to the
    // reflexion node.
    reflexion->setAsGhost(true);

    // add reflexion node to world
    world->addChild(reflexion);    

    // create a symmetry rotation matrix (z-plane)
    cMatrix3d rotRefexion;
    rotRefexion.set(1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, -1.0f);
    reflexion->setRot(rotRefexion);
    reflexion->setPos(0.0f, 0.0f, -2.005f);

    // add objects to the world
    reflexion->addChild(slinghot);
	reflexion->addChild(avatar);
}

void VirtualEnvironment::createRectangle(cMesh* a_mesh, double width, double height, double depth)
{
    const double HALFWIDTH = width / 2.0;
    const double HALFHEIGTH = height / 2.0;
    const double HALFDEPTH = depth / 2.0;
    int vertices [6][6];

    // face -x
    vertices[0][0] = a_mesh->newVertex(-HALFDEPTH,  HALFWIDTH, -HALFHEIGTH);
    vertices[0][1] = a_mesh->newVertex(-HALFDEPTH, -HALFWIDTH, -HALFHEIGTH);
    vertices[0][2] = a_mesh->newVertex(-HALFDEPTH, -HALFWIDTH,  HALFHEIGTH);
    vertices[0][3] = a_mesh->newVertex(-HALFDEPTH,  HALFWIDTH,  HALFHEIGTH);

    // face +x
    vertices[1][0] = a_mesh->newVertex( HALFDEPTH, -HALFWIDTH, -HALFHEIGTH);
    vertices[1][1] = a_mesh->newVertex( HALFDEPTH,  HALFWIDTH, -HALFHEIGTH);
    vertices[1][2] = a_mesh->newVertex( HALFDEPTH,  HALFWIDTH,  HALFHEIGTH);
    vertices[1][3] = a_mesh->newVertex( HALFDEPTH, -HALFWIDTH,  HALFHEIGTH);

    // face -y
    vertices[2][0] = a_mesh->newVertex(-HALFDEPTH,  -HALFWIDTH, -HALFHEIGTH);
    vertices[2][1] = a_mesh->newVertex( HALFDEPTH,  -HALFWIDTH, -HALFHEIGTH);
    vertices[2][2] = a_mesh->newVertex( HALFDEPTH,  -HALFWIDTH,  HALFHEIGTH);
    vertices[2][3] = a_mesh->newVertex(-HALFDEPTH,  -HALFWIDTH,  HALFHEIGTH);

    // face +y
    vertices[3][0] = a_mesh->newVertex( HALFDEPTH,   HALFWIDTH, -HALFHEIGTH);
    vertices[3][1] = a_mesh->newVertex(-HALFDEPTH,   HALFWIDTH, -HALFHEIGTH);
    vertices[3][2] = a_mesh->newVertex(-HALFDEPTH,   HALFWIDTH,  HALFHEIGTH);
    vertices[3][3] = a_mesh->newVertex( HALFDEPTH,   HALFWIDTH,  HALFHEIGTH);

    // face -z
    vertices[4][0] = a_mesh->newVertex(-HALFDEPTH,  -HALFWIDTH, -HALFHEIGTH);
    vertices[4][1] = a_mesh->newVertex(-HALFDEPTH,   HALFWIDTH, -HALFHEIGTH);
    vertices[4][2] = a_mesh->newVertex( HALFDEPTH,   HALFWIDTH, -HALFHEIGTH);
    vertices[4][3] = a_mesh->newVertex( HALFDEPTH,  -HALFWIDTH, -HALFHEIGTH);

    // face +z
    vertices[5][0] = a_mesh->newVertex( HALFDEPTH,  -HALFWIDTH,  HALFHEIGTH);
    vertices[5][1] = a_mesh->newVertex( HALFDEPTH,   HALFWIDTH,  HALFHEIGTH);
    vertices[5][2] = a_mesh->newVertex(-HALFDEPTH,   HALFWIDTH,  HALFHEIGTH);
    vertices[5][3] = a_mesh->newVertex(-HALFDEPTH,  -HALFWIDTH,  HALFHEIGTH);

    // create triangles
    for (int i=0; i<6; i++)
    {
    a_mesh->newTriangle(vertices[i][0], vertices[i][1], vertices[i][2]);
    a_mesh->newTriangle(vertices[i][0], vertices[i][2], vertices[i][3]);
    }

    // set material properties to light gray
    a_mesh->m_material.m_ambient.set(0.5f, 0.5f, 0.5f, 1.0f);
    a_mesh->m_material.m_diffuse.set(0.7f, 0.7f, 0.7f, 1.0f);
    a_mesh->m_material.m_specular.set(1.0f, 1.0f, 1.0f, 1.0f);
    a_mesh->m_material.m_emission.set(0.0f, 0.0f, 0.0f, 1.0f);

    // compute normals
    a_mesh->computeAllNormals();

    // compute collision detection algorithm
   // a_mesh->createAABBCollisionDetector(1.01 * proxyRadius, true, false);
}