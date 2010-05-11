#include "DataPacket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataPacket::DataPacket()
{
}

DataPacket::~DataPacket()
{
	// nothing to delete
}

DataPacketType DataPacket::getType() const
{
	// grab the header from the packet
	DataPacketHeader* header = (DataPacketHeader*) &m_vPacket[0];
	return header->type;
}

bool DataPacket::readPacket(std::vector<BYTE>& input)
{
	if (input.size() < sizeof(DataPacketHeader))
	{
		// all data packets need to have a least a header
		return false;
	}

	// grab the header from the packet
	DataPacketHeader* header = (DataPacketHeader*) &input[0];

	unsigned int size = sizeof(DataPacketHeader) + header->size;

	// the stream needs to be bigger or equal to the packet size for the packet to be complete
	if (size <= input.size())
	{
		// we will use this counter to navigate through the stream of bytes
		unsigned int dataIndex = 0;

		// clear the packet
		m_vPacket.clear();

		// add the header to the packet
		m_vPacket.insert(m_vPacket.end(), &input[dataIndex], &input[dataIndex] + sizeof(DataPacketHeader));
		dataIndex += sizeof(DataPacketHeader);

		// some packets are headers only, make sure there is data before copying it into the packet
		if (header->size > 0)
		{
			// add the data to the packet
			m_vPacket.insert(m_vPacket.end(), &input[dataIndex], &input[dataIndex] + header->size);
			dataIndex += header->size;
		}

		// erase this packet from the input stream
		input.erase(input.begin(), input.begin() + size);

		return true;
	}
	return false;
}

void DataPacket::setVideoData(VideoData video)
{
	// write the video into the packet
	writeData(DATA_PACKET_VIDEO, video);
	return;
}

void DataPacket::setPlayerPosition(const cVector3d& position)
{
	// write the header and data into the packet
	writeData(DATA_PACKET_PLAYER_POSITION, position);
	return;
}

void DataPacket::setSlingshotPosition(const cVector3d& position)
{
	// write the header and data into the packet
	writeData(DATA_PACKET_SLINGSHOT_POSITION, position);
	return;
}

void DataPacket::setProjectile(const Projectile& projectile)
{
	// write the header and data into the packet
	writeData(DATA_PACKET_PROJECTILE, projectile);
	return;
}

void DataPacket::setSlingshotPullback()
{
	// write the header and data into the packet
	writeData(DATA_PACKET_SLINGSHOT_PULLBACK);
	return;
}

void DataPacket::setSlingshotRelease()
{
	// write the header and data into the packet
	writeData(DATA_PACKET_SLINGSHOT_RELEASE);
	return;
}

VideoData DataPacket::getVideoData() const
{
	return readData<VideoData>();	
}

cVector3d DataPacket::getPlayerPosition() const
{
	return readData<cVector3d>();
}

cVector3d DataPacket::getSlingshotPosition() const
{
	return readData<cVector3d>();
}

Projectile DataPacket::getProjectile() const
{
	return readData<Projectile>();
}

const std::vector<BYTE>& DataPacket::getPacket() const
{
	return m_vPacket;
}

void DataPacket::writeData(DataPacketType type)
{
	// clear the existing content
	m_vPacket.clear();

	// create a header for this packet
	DataPacketHeader header;
	header.type = type;
	header.size = 0;

	// copy the header into the packet
	m_vPacket.insert(m_vPacket.end(), (BYTE*) &header, ((BYTE*) &header) + sizeof(DataPacketHeader));

	return;
}

