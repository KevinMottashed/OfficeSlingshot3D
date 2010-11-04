
#include "VirtualEnvironment.h"
#include "WorldConstants.h"

const unsigned int VirtualEnvironment::ball_limit = 2;
const cVector3d VirtualEnvironment::firing_force = cVector3d(0, 300.0f, -450.0f);

VirtualEnvironment::VirtualEnvironment(void) :
	localBalls(ball_limit),
	peerBalls(ball_limit)
{
	world = new cWorld();
	_camera = new cCamera(world);
	light = new cLight(world);
	background = new cBitmap();
	muteControl = new cBitmap();
	volumeControl = new cBitmap();
}

VirtualEnvironment::~VirtualEnvironment(void)
{
	// we only need to delete world because it is the parent object
	// and will delete all its children when it is deleted
	delete world;
}

void VirtualEnvironment::updateFrame()
{
	ODEWorld->updateDynamics(0.02);
	ODEWorld->computeGlobalPositions(true);
}

bool VirtualEnvironment::isColliding()
{
	foreach (VirtualBall*& odeBall, peerBalls)
	{
		if (!odeBall->getAlreadyCollided()) {
			cVector3d ballPos = odeBall->getMeshPos();
			bool collided = lAvatar->isInHitBox(ballPos);

			if(collided) {
				odeBall->collided();
			}
			return collided;
		}
	}
	return false;
}

cCamera* VirtualEnvironment::camera()
{
	return _camera;
}

Projectile VirtualEnvironment::fireLocalSlingshot()
{
	// TODO calulate force and fire
	/*
	cVector3d ballPosition = lSlingshot->getBallPosition();

	// the resulting projectile
	Projectile p;
	p.position(ballPosition);
	p.force(firing_force);

	localBalls[lNumBalls % ball_limit]->fire(p);
	
	// increment the number of balls that were fired, so we know which one to use next
	lNumBalls++;
	*/

	Projectile p;
	return p;
}

void VirtualEnvironment::firePeerSlingshot(Projectile p)
{
	peerBalls[rNumBalls % ball_limit]->fire(p);
	
	// increment the number of balls that were fired, so we know which one to use next
	rNumBalls++;

	return;
}

void VirtualEnvironment::moveLocalAvatar(cVector3d position)
{
	//Calculate angle of rotation, limit to 45 degrees either way
	double ang = cRadToDeg(atan(position.x / position.y));
	ang = cClamp<double>(ang, -45, 45);

	lAvatar->rotate(ang);
	//lAvatar->translate(ang);
	//lAvatar->updateBoundaries(ang, position);
	
	return;
}

void VirtualEnvironment::movePeerAvatar(cVector3d position)
{
	//Calulate angle of rotation, limit to 45 degrees either way
	double ang = cRadToDeg(atan(position.x / position.y));
	ang = cClamp<double>(ang, -45, 45);

	rAvatar->rotate(-ang);
	//rAvatar->translate(ang);

	return;
}


void VirtualEnvironment::reduceLocalHp(unsigned int dmg)
{
	lHpBar->ReduceHP(dmg);
	
	return;
}

void VirtualEnvironment::reducePeerHp(unsigned int dmg)
{
	rHpBar->ReduceHP(dmg);

	return;
}

void VirtualEnvironment::resetAll()
{
	rHpBar->ResetHp();
	lHpBar->ResetHp();

	lAvatar->resetPosition(World::local_avatar_starting_position, true);
	rAvatar->resetPosition(World::peer_avatar_starting_position, false);

	foreach (VirtualBall*& odeBall, localBalls)
	{
		odeBall->reset();
	}

	foreach (VirtualBall*& odeBall, peerBalls)
	{
		odeBall->reset();
	}

	return;
}

bool VirtualEnvironment::isLocalPlayerDead(){
	return (lHpBar->getHP() <= 0);
}

cVector3d VirtualEnvironment::getCurrentBallPosition()
{
	return peerBalls[(rNumBalls-1) % ball_limit]->getMeshPos();
}

int VirtualEnvironment::getAvatarHitBodyPart()
{
	return lAvatar->getHitBodyPart();
}

cVector3d VirtualEnvironment::getLocalAvatarChestMin()
{
	return lAvatar->getChestMin();
}

cVector3d VirtualEnvironment::getLocalAvatarChestMax()
{
	return lAvatar->getChestMax();
}

cVector3d VirtualEnvironment::getLocalAvatarRightArmMin()
{
	return lAvatar->getRightArmMin();
}

cVector3d VirtualEnvironment::getLocalAvatarRightArmMax()
{
	return lAvatar->getRightArmMax();
}

cVector3d VirtualEnvironment::getLocalAvatarLeftArmMin()
{
	return lAvatar->getLeftArmMin();
}

cVector3d VirtualEnvironment::getLocalAvatarLeftArmMax()
{
	return lAvatar->getLeftArmMax();
}

cVector3d VirtualEnvironment::getLocalAvatarHeadMin()
{
	return lAvatar->getHeadMin();
}

cVector3d VirtualEnvironment::getLocalAvatarHeadMax()
{
	return lAvatar->getHeadMax();
}

void VirtualEnvironment::changeMuteIcon(bool soundOn)
{
	if(soundOn) {
		muteControl->m_image.loadFromFile("pictures/mute.tga");
		assert(muteControl->m_image.initialized() == 1);

		volumeControl->m_image.loadFromFile("pictures/vol0.tga");
		assert(volumeControl->m_image.initialized() == 1);
	} else {
		muteControl->m_image.loadFromFile("pictures/sound.tga");
		assert(muteControl->m_image.initialized() == 1);

		volumeControl->m_image.loadFromFile("pictures/vol5.tga");
		assert(volumeControl->m_image.initialized() == 1);
	}
}

void VirtualEnvironment::changeVolumeIcon(int vol)
{
	ostringstream os;
	os << "pictures/vol" << vol << ".tga";
	volumeControl->m_image.loadFromFile(os.str().c_str());
	assert(volumeControl->m_image.initialized() == 1);

	muteControl->m_image.loadFromFile("pictures/sound.tga");
	assert(muteControl->m_image.initialized() == 1);
}

void VirtualEnvironment::initialize(void)
{
	//**************************************//
	//                WORLD                 // 
	//**************************************//

	world->addChild(_camera);

	//**************************************//
	//               CAMERA                 //
	//**************************************//

    // position and oriente the camera
	_camera->set( cVector3d (0.0f, 1.5f, 9.0f),    // camera position (eye)
        cVector3d (0.0f, 0.0f, 0.0f),    // lookat position (target)
        cVector3d (0.0f, 1.0f, 0.0f));   // direction of the "up" vector

    // set the near and far clipping planes of the camera
    // anything in front/behind these clipping planes will not be rendered
    _camera->setClippingPlanes(-9.0f, 9.0f);

    // enable high quality rendering
    _camera->enableMultipassTransparency(false);

	//**************************************//
	//                LIGHT                 //
	//**************************************//

    // create a light source and attach it to the camera
    _camera->addChild(light);                   // attach light to camera
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
	ODEWorld->setGravity(cVector3d(0.0, -9.81, 0.0));

	//**************************************//
	//              SLINGSHOTS              //
	//**************************************//

	// Create both slingshots
	lSlingshot = new VirtualSlingshot(world, World::local_slingshot_position);
	rSlingshot = new VirtualSlingshot(world, World::peer_slingshot_position);

	//**************************************//
	//                AVATARS               //
	//**************************************//

	lAvatar = new VirtualAvatar(world, World::local_avatar_starting_position, true);
	rAvatar = new VirtualAvatar(world, World::peer_avatar_starting_position, false);
	
	//**************************************//
	//                 HP BAR               //
	//**************************************//

	lHpBar = new HpBar(world, true);
	rHpBar = new HpBar(world, false);

	//**************************************//
	//                BALLS                 //
	//**************************************//

	foreach (VirtualBall*& odeBall, localBalls)
	{
		odeBall = new VirtualBall(world, ODEWorld);
	}

	foreach (VirtualBall*& odeBall, peerBalls)
	{
		odeBall = new VirtualBall(world, ODEWorld);
	}

	//**************************************//
	//               GROUND                 //
	//**************************************//

	//Create a static ground plane
    ODEGround = new cODEGenericBody(ODEWorld);
    ODEGround->createStaticPlane(cVector3d(0.0f, 0.0f, 0.0f), cVector3d(0.0f, 1.0f, 0.0f));

	//**************************************//
	//             BACKGROUND               //
	//**************************************//

	cBitmap* background = new cBitmap();
	background->m_image.loadFromFile("Objects\\table3.bmp");

	background->setPos(cVector3d(-13.1f, -10.6f, -9.0f));

	background->setZoomHV(0.5, 0.5);

	world->addChild(background);

	rNumBalls = 0;
	lNumBalls = 0;

	//**************************************//
	//            VOLUME CONTROLS           //
	//**************************************//

	// .tga seems to work better then .bmp
	muteControl->m_image.loadFromFile("pictures/sound.tga");
	assert(muteControl->m_image.initialized() == 1);

	// there might be a better way to add the bitmap
	// so that it fits perfectly, but this is good enough
	_camera->m_front_2Dscene.addChild(muteControl);
	muteControl->setPos(505, 370, 0);

	// .tga seems to work better then .bmp
	volumeControl->m_image.loadFromFile("pictures/vol5.tga");
	assert(volumeControl->m_image.initialized() == 1);

	// there might be a better way to add the bitmap
	// so that it fits perfectly, but this is good enough
	_camera->m_front_2Dscene.addChild(volumeControl);
	volumeControl->setPos(537, 370, 0);
}