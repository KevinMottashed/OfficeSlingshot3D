#ifndef PACKET_H
#define PACKET_H

#include "stdAfx.h" // STL & Windows
#include "chai3d.h" // chai3d
#include "boost.h" // boost

// we need to send some data types from the game module
#include "Projectile.h"
#include "VideoData.h"

/**
 * The header for all packets.
 * The header contains a size which is the size of the whole packet and
 * a type which should be a enum representing what the packet is.
 */
template <typename PacketType>
struct PacketHeader
{
	unsigned int size; /**< The size of the packet */
	PacketType type; /**< The packet type */
};

/** 
 * The Packet class is used to represent all packets that our network can send.
 * All packets include a header and data section. The header is the type of the packet and the
 * size of the data section. This class uses the boost serialization library to serialize
 * types for transmission and deserialize types upon reception. A packet has 2 sections and 
 * they are ordered as follows <header><data>.
 */
template <typename PacketType>
class Packet
{
public:
	/**
	 * Constructor. Creates an empty packet.
	 */
	Packet();

	/**
	 * Destructor.
	 */
	virtual ~Packet();

	/** 
	 * Get the packet's type.
	 * @return the type
	 */
	PacketType getType() const;

	/**
	 * Get the packet's header in bytes.
	 * @return a vector of chars containing the packet's header.
	 */
	boost::shared_ptr<const std::vector<char> > getHeader() const;

	/**
	 * Get the packet's data in bytes.
	 * @return a vector of chars containing the packet's data.
	 */
	boost::shared_ptr<const std::vector<char> > getData() const;

	/**
	 * Attempt to create a data packet from a stream of bytes.
	 * If we could create a packet from the bytes then the contents of this packet will be replaced with contents from the stream.
	 * The bytes that were used to create the packet will also be removed from the stream
	 * the function returns false if a packet could not be created.
	 * @param input The bytes used to try to create a packet.
	 * @return true if a packet was created.
	 */
	bool readPacket(std::vector<char>& input);

	/**
	 * Write data into the packet, including the header.
	 * @param type The type of packet that this will become.
	 * @param t The data to write into the packet.
	 */
	template <typename T>
	void write(PacketType type, const T& t);

	/**
	 * Write video data into the packet.
	 * Video data is considered a special case as the data is so huge.
	 * @param type The type of packet that this will become.
	 * @param video The video to write into the packet.
	 */
	void write(PacketType type, const VideoData& video);

	/**
	 * Write a header into the packet. This version is provided for header only packets.
	 * @param type The type of packet that this will become.
	 */
	void write(PacketType type);

	/**
	 * Read data from the packet.
	 * @param t The extracted data.
	 */
	template <typename T>
	void read(T& t) const;

	/**
	 * Read video data from the packet.
	 * @param t The extracted video.
	 */
	void read(VideoData& video) const;

	/**
	 * Clear the packets header and data.
	 */
	void clear();

private:
	boost::shared_ptr<std::vector<char> > header; /**< The header section */
	boost::shared_ptr<std::vector<char> > data; /**< The data section */
};

//--------------------------------------------
// TEMPLATE IMPLEMENTATIONS
//--------------------------------------------

template <typename PacketType>
Packet<PacketType>::Packet()
{
	header = boost::shared_ptr<std::vector<char> >(new std::vector<char>());
	data = boost::shared_ptr<std::vector<char> >(new std::vector<char>());
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
boost::shared_ptr<const std::vector<char> > Packet<PacketType>::getHeader() const
{
	return header;
}

template <typename PacketType>
boost::shared_ptr<const std::vector<char> > Packet<PacketType>::getData() const
{
	return data;
}

template <typename PacketType>
bool Packet<PacketType>::readPacket(std::vector<char>& input)
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
	header->insert(header->end(), (char*) &temp, ((char*) &temp) + sizeof(PacketHeader<PacketType>));
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

	// serialize the data, note that the archive must be destroyed before we can determine the size
	// of the data, otherwise the data vector will still be empty
	{
		boost::iostreams::filtering_ostream dataOStream(boost::iostreams::back_inserter(*data));
		boost::archive::text_oarchive archive(dataOStream);
		archive << t;
	}

	// we can now calculate the total size of the packet (excluding the header)
	temp.size = data->size();

	// copy the header into the packet
	header->insert(header->end(), (char*) &temp, ((char*) &temp) + sizeof(PacketHeader<PacketType>));

	return;
}

template <typename PacketType>
void Packet<PacketType>::write(PacketType type, const VideoData& video)
{
	// clear the existing content
	clear();

	// create a header for this packet
	PacketHeader<PacketType> temp;
	temp.type = type;

	// serialize the data, note that the archive must be destroyed before we can determine the size
	// of the data, otherwise the data vector will still be empty
	{
		boost::iostreams::filtering_ostream dataOStream(boost::iostreams::back_inserter(*data), ios::out | ios::binary);
		boost::archive::binary_oarchive archive(dataOStream);
		archive << video;
	}

	// we can now calculate the total size of the packet (excluding the header)
	temp.size = data->size();

	// copy the header into the packet
	header->insert(header->end(), (char*) &temp, ((char*) &temp) + sizeof(PacketHeader<PacketType>));

	return;
}


template <typename PacketType>
template <typename T>
void Packet<PacketType>::read(T& t) const
{
	// create a binary stream that will read from the data vector
	boost::iostreams::filtering_istream dataIStream(boost::make_iterator_range(*data));

	// create an archive from the vector stream and read the data
	boost::archive::text_iarchive archive(dataIStream);
	archive >> t;
	return;
}

// The VideoData class requires a special consideration because it is so big.
// We will be using a binary archive, which is more efficient then a text archive
// for this kind of data (array of bytes).
template <typename PacketType>
void Packet<PacketType>::read(VideoData& video) const
{
	// create a binary stream that will read from the data vector
	boost::iostreams::filtering_istream dataIStream(boost::make_iterator_range(*data), ios::in | ios::binary);

	// create an archive from the vector stream and read the data
	boost::archive::binary_iarchive archive(dataIStream);
	archive >> video;
	return;
}

template <typename PacketType>
void Packet<PacketType>::clear()
{
	header->clear();
	data->clear();
	return;
}

#endif

