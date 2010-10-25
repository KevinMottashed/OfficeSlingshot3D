#include "WinScreen.h"

WinScreen::WinScreen() :
	world(new cWorld()),
	_camera(new cCamera(world)),
	light(new cLight(world)),
	bitmap(new cBitmap())
{
	world->addChild(_camera);
	world->setBackgroundColor(0.0, 0.8, 0.0);
	_camera->addChild(light);

	//Create a label with text, or we can use an image
	cLabel* label = new cLabel();
	label->setPos(150,200,0);
	label->m_string = "You == Win";
	(label->m_fontColor).set(0,0,0,1);
	(label->m_font)->setPointSize(300);
	_camera->m_front_2Dscene.addChild(label);
	

	// .tga seems to work better then .bmp
	/*
	bitmap->m_image.loadFromFile("pictures/title.tga");
	assert(bitmap->m_image.initialized() == 1);
	*/

	// there might be a better way to add the bitmap
	// so that it fits perfectly, but this is good enough
	/*
	_camera->m_front_2Dscene.addChild(bitmap);
	bitmap->setPos(0, 0, 0);
	bitmap->setZoomHV(0.7, 0.7);
	*/
}

WinScreen::~WinScreen()
{
	delete world;
}

cCamera* WinScreen::camera()
{
	return _camera;
}


