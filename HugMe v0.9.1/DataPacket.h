// DataPacket.h: interface for the DataPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAPACKET_H__642CEFC6_DF57_4693_9B56_D98227F17BE7__INCLUDED_)
#define AFX_DATAPACKET_H__642CEFC6_DF57_4693_9B56_D98227F17BE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// STL & Windows
#include "StdAfx.h"

enum DataPacketType
{
	DATA_PACKET_VIDEO,
	DATA_PACKET_UNKNOWN
};

// This is the header for all data packets
struct DataPacketHeader
{
	int size;
	DataPacketType type;
};

class DataPacket  
{
public:
	// the default constructor creates an empty packet
	DataPacket();
	virtual ~DataPacket();

	// get the packet's type
	DataPacketType getType() const;

	// Get the RGB video data of the packet in bytes
	const char* getVideoData() const;

	// get the packet
	const std::vector<BYTE>& getPacket() const;

	// attempt to create a data packet from a stream of bytes
	// if we could create a packet from the bytes then the contents of this packet will be replaced with contents from the stream
	// the bytes that were used to create the packet will also be removed from the stream
	// the function returns false if a packet could not be created
	bool readPacket(std::vector<BYTE>& input);

	// set the packet's video data
	void setVideoData(const std::vector<BYTE>& vRGB);

private:
	std::vector<BYTE> m_vPacket;
};

#endif // !defined(AFX_DATAPACKET_H__642CEFC6_DF57_4693_9B56_D98227F17BE7__INCLUDED_)
