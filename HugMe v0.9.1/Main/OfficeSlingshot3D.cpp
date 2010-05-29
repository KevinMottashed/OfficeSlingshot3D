#include "OfficeSlingshot3D.h"

OfficeSlingshot3D::OfficeSlingshot3D(	boost::shared_ptr<Mediator> mediator,
										boost::shared_ptr<Logger> logger, 
										boost::shared_ptr<Replayer> replayer,
										CDialog* dialog) :
	mediator(mediator),
	logger(logger),
	replayer(replayer),
	dialog(dialog)
{
}

OfficeSlingshot3D::~OfficeSlingshot3D()
{
}

void OfficeSlingshot3D::run()
{
	// not all instances have a replayer
	// check before trying to start it
	if (replayer)
	{
		replayer->startReplay();
	}

	// start the UI
	dialog->DoModal();
	return;
}

void OfficeSlingshot3D::operator ()()
{
	run();
	return; 
}