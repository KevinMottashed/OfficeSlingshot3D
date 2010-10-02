#ifndef OFFICE_SLINGSHOT_3D_H
#define OFFICE_SLINGSHOT_3D_H

#include "stdafx.h"
#include "boost.h"
#include "MediatorProxy.h"
#include "NetworkProxy.h"
#include "UserInterfaceProxy.h"
#include "FalconProxy.h"
#include "ZCameraProxy.h"
#include "LoggerProxy.h"
#include "ReplayProxy.h"

// The office slingshot 3D class is the class that runs the game
// The game can be started by calling run() or operator ()
class OfficeSlingshot3D
{
public:
	OfficeSlingshot3D(	boost::shared_ptr<Game> game,
						boost::shared_ptr<Logger> logger, 
						boost::shared_ptr<Replayer> replayer,
						CDialog* dialog);
	~OfficeSlingshot3D();

	// both of these function start the application
	// operator () is provided so that we can easily create a boost::thread from this object
	// as boost::thread expects a functor
	void run();
	void operator()();

private:
	// the different aspects of the program
	// we need to hang on to all of these so that nothing gets destroyed
	boost::shared_ptr<Game> game;
	boost::shared_ptr<Logger> logger;
	boost::shared_ptr<Replayer> replayer;

	// used to start the application
	CDialog* dialog;
};

#endif