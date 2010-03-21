// ControlPacket.h: interface for the ControlPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROLPACKET_H__D6F3D183_3B6D_41A0_AB79_14DEA957D8CE__INCLUDED_)
#define AFX_CONTROLPACKET_H__D6F3D183_3B6D_41A0_AB79_14DEA957D8CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stdafx.h"

enum ControlPacketType
{
	CONTROL_PACKET_NAME, // a user name packet
	CONTROL_PACKET_CHAT, // a chat message packet
	CONTROL_PACKET_UNKNOWN
};

typedef struct ControlPacketHeader
{
	int size; // the size of the packet, excluding the header
	ControlPacketType type; // the type of the packet
} ControlPacketHeader;

class ControlPacket  
{
public:
	ControlPacket();
	virtual ~ControlPacket();

	// get the packet
	const std::vector<BYTE>& getPacket() const;

	// get the user name
	std::string getUserName() const;

	// get the chat message
	std::string getChatMessage() const;

	// get the type of the packet
	ControlPacketType getType() const;

	// set the packet to a user name packet and add the user name to the packet
	void setUserName(const std::string& name);

	// set the packet to a chat message packet and add the message to the packet
	void setChatMessage(const std::string& message);

	// try to create a packet from a series of bytes
	// returns true if we managed to create a packet
	bool readPacket(std::vector<BYTE>& input);

private:
	std::vector<BYTE> m_vPacket;

	// create the packet with the string as the data
	void writeStringPacket(ControlPacketHeader header, const std::string& data);
};

#endif // !defined(AFX_CONTROLPACKET_H__D6F3D183_3B6D_41A0_AB79_14DEA957D8CE__INCLUDED_)
