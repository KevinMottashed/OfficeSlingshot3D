#include "ConsoleLogger.h"

ConsoleLogger::ConsoleLogger()
{
}

ConsoleLogger::~ConsoleLogger()
{
}

void ConsoleLogger::logPeerConnected()
{
	console << "peer connected\n";
	return;
}

void ConsoleLogger::logPeerDisconnected()
{
	console << "peer disconnected\n";
	return;
}

void ConsoleLogger::logPeerStartGame()
{
	console << "peer started game\n";
	return;
}

void ConsoleLogger::logPeerPauseGame()
{
	console << "peer paused game\n";
	return;
}

void ConsoleLogger::logPeerExitGame()
{
	console << "peer exited game\n";
	return;
}

void ConsoleLogger::logNetworkError(rc_network error)
{
	console << "network error " << lookup(error) << "\n";
	return;
}

void ConsoleLogger::logUserNameReceived(const std::string& name)
{
	console << "received user name: " << name << "\n";
	return;
}

void ConsoleLogger::logChatMessageReceived(const std::string& message)
{
	console << "received chat message: " << message << "\n";
	return;
}

void ConsoleLogger::logVideoDataReceived(const VideoData& data)
{
	console << "received video width=" << data.width << " height=" << data.height << "\n";
	return;
}

void ConsoleLogger::logSlingshotPositionReceived(const cVector3d& position)
{
	console << "received slingshot position " << position << "\n";
	return;
}

void ConsoleLogger::logProjectileReceived(const Projectile& projectile)
{
	console << "received projectile " << projectile << "\n";
	return;
}

void ConsoleLogger::logPullbackReceived()
{
	console << "received pullback\n";
	return;
}

void ConsoleLogger::logReleaseReceived()
{
	console << "received release\n";
	return;
}

void ConsoleLogger::logPlayerPositionReceived(const cVector3d& position)
{
	console << "received player position " << position << "\n";
	return;
}

void ConsoleLogger::logConnect()
{
	console << "connect pushed\n";
	return;
}

void ConsoleLogger::logListen()
{
	console << "listen pushed\n";
	return;
}

void ConsoleLogger::logDisconnect()
{
	console << "disconnect pushed\n";
	return;
}

void ConsoleLogger::logChangePreferences(const UserPreferences& preferences)
{
	console << "preferences changed\n" << preferences;
	return;
}

void ConsoleLogger::logLocalStartGame()
{
	console << "start game pushed\n";
	return;
}

void ConsoleLogger::logLocalPauseGame()
{
	console << "pause game pushed\n";
	return;
}

void ConsoleLogger::logLocalExitGame()
{
	console << "exit game pushed\n";
	return;
}

void ConsoleLogger::logCloseApplication()
{
	console << "close application pushed\n";
	return;
}

void ConsoleLogger::logSendChatMessage(const std::string& message)
{
	console << "send chat pushed, message=" << message << "\n";
	return;
}

void ConsoleLogger::logLocalSlingshotPosition(const cVector3d& position)
{
	console << "local slingshot moved to " << position << "\n";
	return;
}

void ConsoleLogger::logLocalVideoData(const VideoData& data)
{
	console << "new local frame, width=" << data.width << " height=" << data.height << "\n";
	return;
}