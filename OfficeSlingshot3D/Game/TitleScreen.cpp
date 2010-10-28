#include "TitleScreen.h"

TitleScreen::TitleScreen() :
	world(new cWorld()),
	_camera(new cCamera(world)),
	light(new cLight(world)),
	bitmap(new cBitmap()),
	muteControl(new cBitmap())
{
	world->addChild(_camera);
	_camera->addChild(light);

	// .tga seems to work better then .bmp
	bitmap->m_image.loadFromFile("pictures/title.tga");
	assert(bitmap->m_image.initialized() == 1);

	// there might be a better way to add the bitmap
	// so that it fits perfectly, but this is good enough
	_camera->m_front_2Dscene.addChild(bitmap);
	bitmap->setPos(0, 0, 0);
	bitmap->setZoomHV(0.7, 0.7);

	// .tga seems to work better then .bmp
	muteControl->m_image.loadFromFile("pictures/sound.tga");
	assert(muteControl->m_image.initialized() == 1);

	// there might be a better way to add the bitmap
	// so that it fits perfectly, but this is good enough
	_camera->m_front_2Dscene.addChild(muteControl);
	muteControl->setPos(570, 370, 0);
}

TitleScreen::~TitleScreen()
{
	delete world;
}

cCamera* TitleScreen::camera()
{
	return _camera;
}

void TitleScreen::changeVolumeIcon(bool soundOn)
{
	if(soundOn) {
		muteControl->m_image.loadFromFile("pictures/mute.tga");
		assert(muteControl->m_image.initialized() == 1);
	} else {
		muteControl->m_image.loadFromFile("pictures/sound.tga");
		assert(muteControl->m_image.initialized() == 1);
	}
}


