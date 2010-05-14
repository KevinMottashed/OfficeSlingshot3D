#ifndef PACKET_H
#define PACKET_H

#include "stdAfx.h" // STL & Windows
#include "chai3d.h" // chai3d
#include "boost.h" // boost

// we need to send some data types from the game module
#include "Projectile.h"
#include "VideoData.h"

#include "Serialization.h" // so we can serialize/deserialize the data

// This is the header for all data packets
template <typename PacketType>
struct PacketHeader
{
	unsigned int size;
	PacketType type;
};

template <typename PacketType>
class Packet  
{
public:
	// the default constructor creates an empty packet
	Packet();
	virtual ~Packet();

	// get the packets type
	PacketType getType() const;

	// get the packet header in bytes
	boost::shared_ptr<std::vector<BYTE> > getHeader() const;

	// get the packet data in bytes
	boost::shared_ptr<std::vector<BYTE> > getData() const;

	// attempt to create a data packet from a stream of bytes
	// if we could create a packet from the bytes then the contents of this packet will be replaced with contents from the stream
	// the bytes that were used to create the packet will also be removed from the stream
	// the function returns false if a packet could not be created
	bool readPacket(std::vector<BYTE>& input);

	// write data into the packet, including the header
	template <typename T>
	void write(PacketType type, const T& data);

	// a version for header only packets
	void write(PacketType type);

	// read data from the packet
	template <typename T>
	T read() const;

	// clear the header and data
	void clear();

private:
	// a packet has 2 sections
	// <header><data>
	boost::shared_ptr<std::vector<BYTE> > header;
	boost::shared_ptr<std::vector<BYTE> > data;	
};

//--------------------------------------------
// TEMPLATE IMPLEMENTATIONS
//--------------------------------------------

template <typename PacketType>
Packet<PacketType>::Packet() :
header(new std::vector<BYTE>()),
data(new std::vector<BYTE>())
{
}

template <typename PacketType>
Packet<PacketType>::~Packet()
{
}

template <typename PacketType>
PacketType Packet<PacketType>::getType() const
{
	// grab the header from the packet
	PacketHeader<PacketType>* tempHeader = (PacketHeader<PacketType>*) &header->front();
	return tempHeader->type;
}

template <typename PacketType>
boost::shared_ptr<std::vector<BYTE> > Packet<PacketType>::getHeader() const
{
	return header;
}

template <typename PacketType>
boost::shared_ptr<std::vector<BYTE> > Packet<PacketType>::getData() const
{
	return data;
}

template <typename PacketType>
bool Packet<PacketType>::readPacket(std::vector<BYTE>& input)
{
	if (input.size() < sizeof(PacketHeader<PacketType>))
	{
		// all data packets need to have a least a header
		return false;
	}

	// grab the header from the packet
	PacketHeader<PacketType>* tempHeader = (PacketHeader<PacketType>*) &input[0];

	// the size of the entire packet (including the header)
	unsigned int size = sizeof(PacketHeader<PacketType>) + tempHeader->size;

	// the stream needs to be bigger or equal to the packet size for the packet to be complete
	if (size <= input.size())
	{
		// we will use this counter to navigate through the stream of bytes
		unsigned int dataIndex = 0;

		// clear the packet
		clear();

		// add the header to the packet
		header->insert(header->end(), &input[dataIndex], &input[dataIndex] + sizeof(PacketHeader<PacketType>));
		dataIndex += sizeof(PacketHeader<PacketType>);

		// some packets are headers only, make sure there is data before copying it into the packet
		if (tempHeader->size > 0)
		{
			// add the data to the packet
			data->insert(data->end(), &input[dataIndex], &input[dataIndex] + tempHeader->size);
			dataIndex += tempHeader->size;
		}

		// erase this packet from the input stream
		input.erase(input.begin(), input.begin() + size);

		return true;
	}
	return false;
}

template <typename PacketType>
void Packet<PacketType>::write(PacketType type)
{
	// clear the existing content
	header->clear();

	// create a header for the packet
	PacketHeader<PacketType> temp;
	temp.type = type;
	temp.size = 0;

	// write it into the packet
	header->insert(header->end(), (BYTE*) &temp, ((BYTE*) &temp) + sizeof(PacketHeader<PacketType>));
	return;
}

template <typename PacketType>
template <typename T>
void Packet<PacketType>::write(PacketType type, const T& t)
{
	// clear the existing content
	clear();

	// create a header for this packet
	PacketHeader<PacketType> temp;
	temp.type = type;

	// serialize the data
	Serialization::serialize(t, data);	

	// we can now calculate the total size of the packet (excluding the header)
	temp.size = data->size();

	// copy the header into the packet
	header->insert(header->end(), (BYTE*) &temp, ((BYTE*) &temp) + sizeof(PacketHeader<PacketType>));

	return;
}

template <typename PacketType>
template <typename T>
T Packet<PacketType>::read() const
{
	T t;

	// deserialize the packet into the data
	Serialization::deserialize(data, t);

	return t;
}

template <typename PacketType>
void Packet<PacketType>::clear()
{
	header->clear();
	data->clear();
	return;
}

#endif
 
