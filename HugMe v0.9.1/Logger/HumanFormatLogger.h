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
	HumanFormatLogger(S& ostream); // for streams that can be copied
	HumanFormatLogger(T param); // for streams that cannot be copied
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

// some typedefs to make things easier
typedef HumanFormatLogger<std::ofstream, char*> HumanFormatFileLogger;
typedef HumanFormatLogger<ConsoleStream> HumanFormatConsoleLogger;

//---------------------------------------------
// Implementation
//---------------------------------------------

template <typename S, typename T>
HumanFormatLogger<S,T>::HumanFormatLogger(S& ostream) : ostream(ostream)
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
	ostream << "peer connected\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logPeerDisconnected()
{
	ostream << "peer disconnected\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logPeerStartGame()
{
	ostream << "peer started game\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logPeerPauseGame()
{
	ostream << "peer paused game\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logPeerExitGame()
{
	ostream << "peer exited game\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logNetworkError(rc_network error)
{
	ostream << "network error " << lookup(error) << "\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logUserNameReceived(const std::string& name)
{
	ostream << "received user name: " << name << "\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logChatMessageReceived(const std::string& message)
{
	ostream << "received chat message: " << message << "\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logVideoDataReceived(const VideoData& data)
{
	ostream << "received video width=" << data.width << " height=" << data.height << "\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logSlingshotPositionReceived(const cVector3d& position)
{
	ostream << "received slingshot position " << position << "\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logProjectileReceived(const Projectile& projectile)
{
	ostream << "received projectile " << projectile << "\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logPullbackReceived()
{
	ostream << "received pullback\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logReleaseReceived()
{
	ostream << "received release\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logPlayerPositionReceived(const cVector3d& position)
{
	ostream << "received player position " << position << "\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logConnect()
{
	ostream << "connect pushed\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logListen()
{
	ostream << "listen pushed\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logDisconnect()
{
	ostream << "disconnect pushed\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logChangePreferences(const UserPreferences& preferences)
{
	ostream << "preferences changed\n" << preferences;
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logLocalStartGame()
{
	ostream << "start game pushed\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logLocalPauseGame()
{
	ostream << "pause game pushed\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logLocalExitGame()
{
	ostream << "exit game pushed\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logCloseApplication()
{
	ostream << "close application pushed\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logSendChatMessage(const std::string& message)
{
	ostream << "send chat pushed, message=" << message << "\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logLocalSlingshotPosition(const cVector3d& position)
{
	ostream << "local slingshot moved to " << position << "\n";
	return;
}

template <typename S, typename T>
void HumanFormatLogger<S,T>::logLocalVideoData(const VideoData& data)
{
	ostream << "new local frame, width=" << data.width << " height=" << data.height << "\n";
	return;
}

#endif