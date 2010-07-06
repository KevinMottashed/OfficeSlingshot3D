#include "VirtualEnvironment.h"

VirtualEnvironment::VirtualEnvironment(void)
{
}

VirtualEnvironment::~VirtualEnvironment(void)
{
}

void VirtualEnvironment::initialize(cWorld* world, cCamera* camera){

	//**************************************//
	//                WORLD                 //
	//**************************************//

    // set the background color of the environment
    // the color is defined by its (R,G,B) components.
    world->setBackgroundColor(0.5, 0.37, 0.28);

	//**************************************//
	//               CAMERA                 //
	//**************************************//

    // position and oriente the camera
    camera->set( cVector3d (7.0, 0.0, 0.3),    // camera position (eye)
        cVector3d (0.0, 0.0, 0.0),    // lookat position (target)
        cVector3d (0.0, 0.0, 1.0));   // direction of the "up" vector

    // set the near and far clipping planes of the camera
    // anything in front/behind these clipping planes will not be rendered
    camera->setClippingPlanes(0.01, 1000.0);

    // enable high quality rendering
    camera->enableMultipassTransparency(true);

	//**************************************//
	//                LIGHT                 //
	//**************************************//

    // create a light source and attach it to the camera
    cLight* light = new cLight(world);
    camera->addChild(light);                   // attach light to camera
    light->setEnabled(true);                   // enable light source
    light->setPos(cVector3d( 2.0, 0.5, 1.0));  // position the light source
    light->setDir(cVector3d(-2.0, 0.5, 1.0));  // define the direction of the light beam
    light->m_ambient.set(0.6, 0.6, 0.6);
    light->m_diffuse.set(0.8, 0.8, 0.8);
    light->m_specular.set(0.8, 0.8, 0.8);

	//**************************************//
	//              SLINGSHOT               //
	//**************************************//

	cMesh* slinghot = new cMesh(world);

    // add object to world
    world->addChild(slinghot);

    slinghot->setPos(1.0, 0.0, -0.5);

	slinghot->rotate( cVector3d(0, 1, 0), cDegToRad(90));
	slinghot->rotate( cVector3d(1, 0, 0), cDegToRad(90));

	bool test = slinghot->loadFromFile("Objects\\slingshot\\slingshot.obj");
	slinghot->scale(4);

    // compute a boundary box
    slinghot->computeBoundaryBox(true);

    // define some haptic friction properties
    slinghot->setFriction(0.1, 0.2, true);

	slinghot->setUseCulling(false, true);

	//**************************************//
	//               GROUND                 //
	//**************************************//

	cMesh* ground = new cMesh(world);
    world->addChild(ground);

    // create 4 vertices (one at each corner)
    double groundSize = 10.0;
    int vertices0 = ground->newVertex(-groundSize, -groundSize, 0.0);
    int vertices1 = ground->newVertex( groundSize, -groundSize, 0.0);
    int vertices2 = ground->newVertex( groundSize,  groundSize, 0.0);
    int vertices3 = ground->newVertex(-groundSize,  groundSize, 0.0);

    // compose surface with 2 triangles
    ground->newTriangle(vertices0, vertices1, vertices2);
    ground->newTriangle(vertices0, vertices2, vertices3);

    // compute surface normals
    ground->computeAllNormals();

    // position ground at the right level
    ground->setPos(0.0, 0.0, -1.0);

    // define some material properties and apply to mesh
    cMaterial matGround;
    matGround.setDynamicFriction(0.7);
    matGround.setStaticFriction(1.0);
    matGround.m_ambient.set(0.0, 0.0, 0.0);
    matGround.m_diffuse.set(0.0, 0.0, 0.0);
    matGround.m_specular.set(0.0, 0.0, 0.0);
    ground->setMaterial(matGround);

    // enable and set transparency level of ground
    ground->setTransparencyLevel(0.75);
    ground->setUseTransparency(true);

	//**************************************//
	//              REFLEXION               //
	//**************************************//

	// we create an intermediate node to which we will attach
    // a copy of the object located inside the world
    cGenericObject* reflexion = new cGenericObject();

    // set this object as a ghost node so that no haptic interactions or
    // collision detecting take place within the child nodes added to the
    // reflexion node.
    reflexion->setAsGhost(true);

    // add reflexion node to world
    world->addChild(reflexion);    

    // create a symmetry rotation matrix (z-plane)
    cMatrix3d rotRefexion;
    rotRefexion.set(1.0, 0.0, 0.0,
                    0.0, 1.0, 0.0,
                    0.0, 0.0, -1.0);
    reflexion->setRot(rotRefexion);
    reflexion->setPos(0.0, 0.0, -2.005);

    // add objects to the world
    reflexion->addChild(slinghot);
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