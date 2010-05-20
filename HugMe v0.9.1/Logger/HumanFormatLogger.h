#ifndef HUMAN_FORMAT_LOGGER_H
#define HUMAN_FORMAT_LOGGER_H

#include "stdafx.h"
#include "Logger.h"
#include "ConsoleStream.h"

// concrete logger class
// this logger outputs the log in a human readable format
// S is the stream type
// T is the type needed to initialize the stream
template <typename S, typename T = int>
class HumanFormatLogger : public Logger
{
public:
	HumanFormatLogger();
	HumanFormatLogger(T param);
	virtual ~HumanFormatLogger();

protected:
	// log network events
	virtual void logPeerConnected();
	virtual void logPeerDisconnected();
	virtual void logPeerStartGame();
	virtual void logPeerPauseGame();
	virtual void logPeerExitGame();
	virtual void logNetworkError(rc_network error);
	virtual void logUserNameReceived(const std::string& name);
	virtual void logChatMessageReceived(const std::string& message);
	virtual void logVideoDataReceived(const VideoData& data);
	virtual void logSlingshotPositionReceived(const cVector3d& position);
	virtual void logProjectileReceived(const Projectile& projectile);
	virtual void logPullbackReceived();
	virtual void logReleaseReceived();
	virtual void logPlayerPositionReceived(const cVector3d& position);

	// log user interface events
	virtual void logConnect();
	virtual void logListen();
	virtual void logDisconnect();
	virtual void logChangePreferences(const UserPreferences& preferences);
	virtual void logLocalStartGame();
	virtual void logLocalPauseGame();
	virtual void logLocalExitGame();
	virtual void logCloseApplication();
	virtual void logSendChatMessage(const std::string& message);

	// log falcon events
	virtual void logLocalSlingshotPosition(const cVector3d& position);

	// log zcam events
	virtual void logLocalVideoData(const VideoData& data);

private:
	HumanFormatLogger(const HumanFormatLogger& c); // intentionally not implemented
	HumanFormatLogger& operator=(const HumanFormatLogger& c); // intentionally not implemented

	S ostream;
};

//---------------------------------------------
// Template Implementation
//---------------------------------------------

template <typename S, typename T>
HumanFormatLogger<S,T>::HumanFormatLogger() : ostream()
{
}

template <typename S, typename T>
HumanFormatLogger<S,T>::HumanFormatLogger(T param) : ostream(param)
{
}

template <typename S, typename T>
HumanFormatLogger<S,T>::~HumanFormatLogger()
{
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logPeerConnected()
{
	ostream << "peer connected" << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logPeerDisconnected()
{
	ostream << "peer disconnected" << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logPeerStartGame()
{
	ostream << "peer started game" << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logPeerPauseGame()
{
	ostream << "peer paused game" << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logPeerExitGame()
{
	ostream << "peer exited game" << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logNetworkError(rc_network error)
{
	ostream << "network error " << lookup(error) << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logUserNameReceived(const std::string& name)
{
	ostream << "received user name: " << name << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logChatMessageReceived(const std::string& message)
{
	ostream << "received chat message: " << message << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logVideoDataReceived(const VideoData& data)
{
	ostream << "received video width=" << IMAGE_WIDTH << " height=" << IMAGE_HEIGHT << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logSlingshotPositionReceived(const cVector3d& position)
{
	ostream << "received slingshot position " << position << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logProjectileReceived(const Projectile& projectile)
{
	ostream << "received projectile " << projectile << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logPullbackReceived()
{
	ostream << "received pullback" << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logReleaseReceived()
{
	ostream << "received release" << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logPlayerPositionReceived(const cVector3d& position)
{
	ostream << "received player position " << position << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logConnect()
{
	ostream << "connect pushed" << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logListen()
{
	ostream << "listen pushed" << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logDisconnect()
{
	ostream << "disconnect pushed" << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logChangePreferences(const UserPreferences& preferences)
{
	ostream << "preferences changed" << preferences << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logLocalStartGame()
{
	ostream << "start game pushed" << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logLocalPauseGame()
{
	ostream << "pause game pushed" << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logLocalExitGame()
{
	ostream << "exit game pushed" << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logCloseApplication()
{
	ostream << "close application pushed" << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logSendChatMessage(const std::string& message)
{
	ostream << "send chat pushed, message=" << message << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logLocalSlingshotPosition(const cVector3d& position)
{
	ostream << "local slingshot moved to " << position << std::endl;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logLocalVideoData(const VideoData& data)
{
	ostream << "new local frame, width=" << IMAGE_WIDTH << " height=" << IMAGE_HEIGHT << std::endl;
	return;
}

#endif

