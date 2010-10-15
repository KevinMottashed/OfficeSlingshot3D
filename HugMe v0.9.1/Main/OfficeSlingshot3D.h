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

/**
 * The office slingshot 3D class is the class that runs the game
 * The game can be started by calling run() or operator ()
 */
class OfficeSlingshot3D
{
public:
	/**
	 * Constructor
	 * @param game The game object
	 * @param logger The application's logger
	 * @param replayer The application's replayer
	 * @param dialog The main window of the application
	 */
	OfficeSlingshot3D(	boost::shared_ptr<Game> game,
						boost::shared_ptr<Logger> logger, 
						boost::shared_ptr<Replayer> replayer,
						CDialog* dialog);

	/**
	 * Destructor
	 */
	~OfficeSlingshot3D();

	/**
	 * Starts the application
	 */
	void run();

	/**
	 * Starts the application
	 * Provided so that we can easily create a boost::thread from this object
	 */
	void operator()();

private:
	/**
	 * The game object
	 */
	boost::shared_ptr<Game> game;

	/**
	 * The logger object
	 */
	boost::shared_ptr<Logger> logger;

	/**
	 * The replayer object
	 */
	boost::shared_ptr<Replayer> replayer;

	/**
	 * Used to start the application
	 */
	CDialog* dialog;
};

#endif