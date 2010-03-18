// ControlPacket.h: interface for the ControlPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROLPACKET_H__D6F3D183_3B6D_41A0_AB79_14DEA957D8CE__INCLUDED_)
#define AFX_CONTROLPACKET_H__D6F3D183_3B6D_41A0_AB79_14DEA957D8CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ControlPacket  
{
public:
	enum PacketType
	{
		PACKET_NAME,
		PACKET_CHAT
	};

	ControlPacket();
	virtual ~ControlPacket();
};

#endif // !defined(AFX_CONTROLPACKET_H__D6F3D183_3B6D_41A0_AB79_14DEA957D8CE__INCLUDED_)
