#ifndef DATA_PACKET_H
#define DATA_PACKET_H

// STL & Windows
#include "stdAfx.h"

// chai3d
#include "chai3d.h"

// we need to send some data types from the game module
#include "Projectile.h"
#include "VideoData.h"

#include "Serialization.h" // so we can serialize/deserialize the data

enum DataPacketType
{
	DATA_PACKET_VIDEO,
	DATA_PACKET_PLAYER_POSITION,
	DATA_PACKET_SLINGSHOT_POSITION,
	DATA_PACKET_SLINGSHOT_PULLBACK,
	DATA_PACKET_SLINGSHOT_RELEASE,
	DATA_PACKET_PROJECTILE,
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
	VideoData getVideoData() const;

	// Get the players position
	cVector3d getPlayerPosition() const;

	// Get the slingshot position
	cVector3d getSlingshotPosition() const;

	// Get the projectile
	Projectile getProjectile() const;

	// get the packet
	const std::vector<BYTE>& getPacket() const;

	// attempt to create a data packet from a stream of bytes
	// if we could create a packet from the bytes then the contents of this packet will be replaced with contents from the stream
	// the bytes that were used to create the packet will also be removed from the stream
	// the function returns false if a packet could not be created
	bool readPacket(std::vector<BYTE>& input);

	// set the packet's video data
	void setVideoData(VideoData video);

	// set the packet's player position data
	void setPlayerPosition(const cVector3d& position);

	// set the slingshot position data
	void setSlingshotPosition(const cVector3d& position);

	// set the projectile data
	void setProjectile(const Projectile& projectile);

	// make this packet a slingshot pullback packet
	void setSlingshotPullback();

	// make this packet a slingshot release packet
	void setSlingshotRelease();

private:
	std::vector<BYTE> m_vPacket;

	// write data into the packet, including the header
	template <typename T>
	void writeData(DataPacketType type, const T& data);

	// a version for header only packets
	void writeData(DataPacketType type);

	// read data from the packet
	template <typename T>
	T readData() const;
};

template <typename T>
void DataPacket::writeData(DataPacketType type, const T& data)
{
	// clear the existing content
	m_vPacket.clear();

	// create a header for this packet
	DataPacketHeader header;
	header.type = type;

	// serialize the data
	std::vector<BYTE> bytes;
	Serialization::serialize(data, bytes);

	// we now know the size of the data
	header.size = bytes.size();

	// copy the header into the packet
	m_vPacket.insert(m_vPacket.end(), (BYTE*) &header, ((BYTE*) &header) + sizeof(DataPacketHeader));

	// copy the serialized data into the packet
	m_vPacket.insert(m_vPacket.end(), bytes.begin(), bytes.end());	

	return;
}

template <typename T>
T DataPacket::readData() const
{
	T data;

	// deserialize the packet into the data
	Serialization::deserialize(m_vPacket.begin() + sizeof(DataPacketHeader), m_vPacket.end(), data);

	return data;
}

#endif
 
