// DataPacket.h: interface for the DataPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAPACKET_H__642CEFC6_DF57_4693_9B56_D98227F17BE7__INCLUDED_)
#define AFX_DATAPACKET_H__642CEFC6_DF57_4693_9B56_D98227F17BE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// STL
#include <vector>
#include <memory>

// This is the header for all data packets
// The tactile section of the packet might disappear in the future
// each player will calculate the projectile hits so there will be no need to send it to the other player
// we might keep it to test the smart jacket/armband
typedef struct DataPacketHeader
{
	bool	isVideo;
	bool	isTactile;
	int		ImageSize;
	int		DepthSize;
	int		ARDataSize;
	int		TactDataSize;
} DataPacketHeader;

class DataPacket  
{
public:
	// the default constructor creates an empty packet
	DataPacket();
	virtual ~DataPacket();

	// get the packet's header, the header describes which kind of packet it is
	// and how big each section of the packet is
	const DataPacketHeader& getHeader() const;

	// Get the RGB video section of the packet in bytes
	const std::vector<BYTE>& getVideoRGBData() const;

	// get the depth video section of the packet in bytes
	const std::vector<BYTE>& getVideoDepthData() const;

	// get the AR video section of the packet in bytes
	const std::vector<BYTE>& getVideoARData() const;

	// get the tactile data section of the packet in bytes
	const std::vector<BYTE>& getTactileData() const;

	// Assembles the packet and returns it in bytes
	// the assembled packet is the header followed by all the sections in the proper order
	// the format is <header><video RGB><video Depth><video AR><tactile>
	// we return the data with the auto pointer because we don't want to copy the packet (it can be big)
	std::auto_ptr<std::vector<BYTE> > getPacket() const;

	// attempt to create a data packet from a stream of bytes
	// if we could create a packet from the bytes then the contents of this packet will be replaced with contents from the stream
	// the bytes that were used to create the packet will also be removed from the stream
	// the function returns false if a packet could not be created
	bool readPacket(std::vector<BYTE>& input);

	// set the packet's video data
	// currently all video packets need rgb, depth and ar data,
	// this might change in the future
	void setVideoData(const std::vector<BYTE>& vRGB, const std::vector<BYTE>& vDepth, const std::vector<BYTE>& vARData);

	// set the packet's tactile data
	void setTactileData(const std::vector<BYTE>& vTactileData);

private:
	// the packet's header
	DataPacketHeader m_header;

	// the video's RGB data
	std::vector<BYTE> m_vVideoRGBData;

	// the video's Depth data
	std::vector<BYTE> m_vVideoDepthData;

	// the video's AR data (markers for body parts)
	std::vector<BYTE> m_vVideoARData;

	// the tactile data
	std::vector<BYTE> m_vTactileData;
};

#endif // !defined(AFX_DATAPACKET_H__642CEFC6_DF57_4693_9B56_D98227F17BE7__INCLUDED_)
