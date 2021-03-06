#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "stdafx.h" // STL + windows
#include "boost.h" // boost
#include "NetworkProxy.h"
#include "UserInterfaceProxy.h"
#include "FalconProxy.h"
#include "ZCameraProxy.h"
#include "JacketProxy.h"
#include "Configuration.h"
#include "SyncLocker.h"
#include "AudioProxy.h"
#include "MediatorSubject.h"
#include "PerspectiveMath.h"
#include "PhysicsSync.h"

/**
 * @ingroup Mediator
 * @b public
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
	/**
	 * Constructor.
	 * @param[in] network The network component to be mediated.
	 * @param[in] falcon The falcon component to be mediated.
	 * @param[in] zcamera The zcamera component to be mediated.
	 * @param[in] userInterface The user interface component to be mediated.
	 * @param[in] configuration The configuration to use.
	 */
	Mediator(	boost::shared_ptr<Network> network,
				boost::shared_ptr<Falcon> falcon,
				boost::shared_ptr<IZCamera> zcamera,
				boost::shared_ptr<UserInterface> userInterface,
				boost::shared_ptr<Configuration> configuration);
	/**
	 * Destructor.
	 */
	virtual ~Mediator();

	/**
	 * Handle an update from the network component.
	 * @param[in] context The update's context.
	 * @param[in] data The data associated with the update.
	 */ 
	virtual void update(NetworkUpdateContext context, const void* data);

	/**
	 * Handle an update from the user interface component.
	 * @param[in] context The update's context.
	 * @param[in] data The data associated with the update.
	 */ 
	virtual void update(UserInterfaceUpdateContext context, const void* data);
	
	/**
	 * Handle an update from the zcamera component.
	 * @param[in] context The update's context.
	 * @param[in] data The data associated with the update.
	 */ 
	virtual void update(ZCameraUpdateContext context, const void* data);
	
	/**
	 * Handle an update from the falcon component.
	 * @param[in] context The update's context.
	 * @param[in] data The data associated with the update.
	 */ 
	virtual void update(FalconUpdateContext context, const void* data);

	/**
	 * Take action when the game is started.
	 * This will start polling the devices and let the user know that the game was started.
	 * @param[in] player the player that started the game.
	 */
	void startGame(Player_t player);

	/**
	 * Take action when the game is paused.
	 * This will stop polling the devices and let the user know that the game was paused.
	 * @param[in] player the player that started the game.
	 */
	void pauseGame(Player_t player);
	
	/**
	 * Take action when the game is exited.
	 * This will stop polling the devices and let the user know that the game was exited.
	 * @param[in] player the player that started the game.
	 */	
	void exitGame(Player_t player);

	/**
	 * Switches the camera whose frames need to be displayed.
	 * This instructs the UI to display the game environment from a camera.
	 * @param[in] camera The new camera.
	 */
	void switchCamera(cCamera* camera);

	/**
	 * Provide the user with feedback when a slingshot is fired.
	 * This will also let the peer know that a slingshot was fired.
	 * if the fired slingshot was our own.
	 * @param[in] projectile The projectile that was fired.
	 * @param[in] player The player that fired his slingshot.
	 */
	void fireSlingshot(Projectile projectile, Player_t player);

	/**
	 * Provide the appropriate user feedback for when the local player is hit by the slingshot.
	 * @param[in] hitPart The body part hit by the ball.
	 * @param[in] ballPos The position of the ball when the collision was detected.
	 * @param[in] minValue The hit box's minimum value.
	 * @param[in] maxValue The hit box's maximum value.
	 */
	void collisionDetected(	BodyPart_t hitPart, 
							cVector3d ballPos, 
							cVector3d minValue, 
							cVector3d maxValue);

	/**
	 * Provide the appropriate user feedback for when the peer player is hit by the slingshot.
	 * @param[in] bodyPart The body part where the peer was hit.
	 */
	void peerHit(BodyPart_t bodyPart);

	/**
	 * Displays lose screen and sends GAME_OVER to peer.
	 * @param[in] player The player that lost the game.
	 */
	void playerLost(Player_t player);

	/**
	 * Mutes the volume.
	 * @param[in] soundOn True if the sound is on currently and is to be muted.
	 */
	void muteVolume(const bool soundOn);

	/**
	 * Changes the volume.
	 * @param[in] vol The new volume.
	 */
	void changeVolume(const int vol);

	/**
	 * Sets the config volume preferences to the game and UI.
	 */
	void setVolumePreferences();

	/**
	 * Redraw the screen.
	 */
	void paint();

	/**
	 * Synchronize the physics of both games.
	 * @param[in] sync The physics data that is used for the synchronization.
	 */
	void synchronizePhysics(const PhysicsSync& sync);
	
private:
	/**
	 * Copy constructor. Not implemented to protect from use.
	 * @param[in] c The mediator to copy from.
	 */	
	Mediator(const Mediator& c);

	/**
	 * Assignment Operator. Not implemented to protect from use.
	 * @param[in] c The mediator to assign from.
	 */
	Mediator& operator=(const Mediator& c); 

	/**
	 * The network component.
	 */
	boost::shared_ptr<Network> network;

	/**
	 * The user interface component.
	 */
	boost::shared_ptr<UserInterface> userInterface;

	/**
	 * The falcon component.
	 */
	boost::shared_ptr<Falcon> falcon;

	/**
	 * The zcamera component.
	 */
	boost::shared_ptr<IZCamera> zcamera;

	/**
	 * The smart clothing component.
	 */
	boost::shared_ptr<SmartClothingManager> smartClothing;

	/**
	 * The audio component.
	 */
	Audio audio;

	/**
	 * The user preferences.
	 */
	boost::shared_ptr<Configuration> configuration;

	/**
	 * Mutex for the user preferences.
	 */
	mutable CRITICAL_SECTION configurationMutex;

	//--------------------------------------------
	// Network Related updates
	//--------------------------------------------

	/**
	 * Handle a peer connect update.
	 */
	void handlePeerConnected();

	/**
	 * Handle a peer disconnect update.
	 */
	void handlePeerDisconnected();

	/**
	 * Handle a network error.
	 * @param[in] error The error code to handle.
	 */
	void handleNetworkError(rc_network error);

	/**
	 * Handle receiving a user name through the network.
	 * @param[in] name The received user name.	
	 */ 
	void handleUserName(const std::string& name);

	/**
	 * Handle a chat message received through the network.
	 * @param[in] message The chat message received.
	 */
	void handleChatMessage(const std::string& message);

	//--------------------------------------------
	// User Interface Related updates
	//--------------------------------------------

	/**
	 * Connect to the peer.
	 * The IP address to connect to is retrieved from the user preferences.
	 */
	void connect();

	/**
	 * Listen for connections.
	 */
	void listen();

	/**
	 * Disconnect us from the peer.
	 * If the network is currently listening for connections then it will stop doing so.
	 */
	void disconnect();

	/**
	 * Handle a change in user preferences.
	 * @param[in] preferences The new preferences.
	 */
	void changePreferences(const UserPreferences& preferences);

	/**
	 * Handle a request to close the application.
	 */
	void closeApplication();

	/**
	 * Send a chat message to the peer.
	 * @param[in] message The chat message to send.
	 */
	void sendChatMessage(const std::string& message);

	//--------------------------------------------
	// ZCamera Related updates
	//--------------------------------------------

	/**
	 * Stop polling the devices (slingshot + zcamera).
	 */
	void stopDevices();

};

#endif

