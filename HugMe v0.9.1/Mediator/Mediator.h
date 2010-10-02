#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "stdafx.h" // STL + windows
#include "boost.h" // boost
#include "NetworkProxy.h"
#include "UserInterfaceProxy.h"
#include "FalconProxy.h"
#include "ZCameraProxy.h"
#include "JacketProxy.h"
#include "VideoData.h"
#include "Configuration.h"
#include "SyncLocker.h"
#include "AudioProxy.h"
#include "MediatorSubject.h"

/**
 * This class mediates the interactions between the devices and the game.
 * All device modules report events to the mediator. The mediator will then
 * interpret these events and notify the game. The mediator also synchronizes
 * both clients via the network connection.
 */
class Mediator :	public NetworkObserver,
					public UserInterfaceObserver,
					public ZCameraObserver,
					public FalconObserver,
					public MediatorSubject
{
public:
	Mediator(	boost::shared_ptr<Network> network,
				boost::shared_ptr<Falcon> falcon,
				boost::shared_ptr<IZCamera> zcamera,
				boost::shared_ptr<UserInterface> userInterface,
				boost::shared_ptr<Configuration> configuration);
	virtual ~Mediator();

	// updates for observer patterns
	virtual void update(NetworkUpdateContext context, const void* data);
	virtual void update(UserInterfaceUpdateContext context, const void* data);
	virtual void update(ZCameraUpdateContext context, const void* data);
	virtual void update(FalconUpdateContext context, const void* data);

	/**
	 * Take action when the game is started.
	 * This will start polling the devices and let the user know that the game was started.
	 * @param player the player that started the game
	 */
	void startGame(Player_t player);

	/**
	 * Take action when the game is paused.
	 * This will stop polling the devices and let the user know that the game was paused.
	 * @param player the player that started the game
	 */
	void pauseGame(Player_t player);
	
	/**
	 * Take action when the game is exited.
	 * This will stop polling the devices and let the user know that the game was exited.
	 * @param player the player that started the game
	 */	
	void exitGame(Player_t player);

	/**
	 * Switches the camera whose frames need to be displayed.
	 * This instructs the UI to display the game environment from a camera.
	 * @param camera The new camera
	 */
	void switchCamera(cCamera* camera);


private:
	Mediator(const Mediator& c); // intentionally not implemented
	Mediator& operator=(const Mediator& c); // intentionally not implemented

	// managers for the different modules
	boost::shared_ptr<Network> network;
	boost::shared_ptr<UserInterface> userInterface;
	boost::shared_ptr<Falcon> falcon;
	boost::shared_ptr<IZCamera> zcamera;
	boost::shared_ptr<SmartClothingManager> smartClothing;
	Audio audio;

	// the user preferences
	boost::shared_ptr<Configuration> configuration;
	mutable CRITICAL_SECTION configurationMutex;

	//--------------------------------------------
	// Network Related updates
	//--------------------------------------------

	// connection status
	void handlePeerConnected();
	void handlePeerDisconnected();
	void handleNetworkError(rc_network error);

	// data reception
	void handleUserName(const std::string& name);
	void handleChatMessage(const std::string& message);

	//--------------------------------------------
	// User Interface Related updates
	//--------------------------------------------

	// connection status updates
	// the user wants to change the connection status
	void connect();
	void listen();
	void disconnect();

	// the user changed his preferences
	void changePreferences(const UserPreferences& preferences);

	// user wants to close the application
	void closeApplication();
	void sendChatMessage(const std::string& message);

	//--------------------------------------------
	// ZCamera Related updates
	//--------------------------------------------

	// new local video
	void handleLocalVideoData(VideoData& video);

	//--------------------------------------------
	// Falcon Related updates
	//--------------------------------------------

	// notify that a new ball was shot
	void notifyNewBallShot(const cVector3d& force);
	void receiveNewBall(const cVector3d& force);

	/**
	 * Stop polling the devices (slingshot + zcamera).
	 */
	void stopDevices();

};

#endif

