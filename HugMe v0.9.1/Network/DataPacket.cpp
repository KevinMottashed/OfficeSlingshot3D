// DataPacket.cpp: implementation of the DataPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "DataPacket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataPacket::DataPacket()
{
	// start off with an unknown packet of size 0
	DataPacketHeader header;
	header.size = 0;
	header.type = DATA_PACKET_UNKNOWN;
	m_vPacket.insert(m_vPacket.end(), (BYTE*) &header, ((BYTE*) &header) + sizeof(DataPacketHeader));
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

void DataPacket::setVideoData(const char* pVideoData, unsigned int size)
{
	DataPacketHeader header;
	header.type = DATA_PACKET_VIDEO;
	header.size = size;

	// clear the existing content
	m_vPacket.clear();

	// copy the header into the packet
	m_vPacket.insert(m_vPacket.end(), (BYTE*) &header, ((BYTE*) &header) + sizeof(DataPacketHeader));

	// copy the video data into the packet
	m_vPacket.insert(m_vPacket.end(), pVideoData, pVideoData + size);

	return;
}

void DataPacket::setPlayerPosition(const cVector3d& position)
{
	DataPacketHeader header;
	header.type = DATA_PACKET_PLAYER_POSITION;
	header.size = sizeof(cVector3d);

	// clear the existing content
	m_vPacket.clear();

	// copy the header into the packet
	m_vPacket.insert(m_vPacket.end(), (BYTE*) &header, ((BYTE*) &header) + sizeof(DataPacketHeader));

	// copy the player position into the packet
	m_vPacket.insert(m_vPacket.end(), (BYTE*) &position, ((BYTE*) &position) + sizeof(cVector3d));

	return;
}

void DataPacket::setSlingshotPosition(const cVector3d& position)
{
	DataPacketHeader header;
	header.type = DATA_PACKET_SLINGSHOT_POSITION;
	header.size = sizeof(cVector3d);

	// clear the existing content
	m_vPacket.clear();

	// copy the header into the packet
	m_vPacket.insert(m_vPacket.end(), (BYTE*) &header, ((BYTE*) &header) + sizeof(DataPacketHeader));

	// copy the slingshot position into the packet
	m_vPacket.insert(m_vPacket.end(), (BYTE*) &position, ((BYTE*) &position) + sizeof(cVector3d));

	return;
}

void DataPacket::setProjectile(const Projectile& projectile)
{
	DataPacketHeader header;
	header.type = DATA_PACKET_PROJECTILE;
	header.size = sizeof(Projectile);

	// clear the existing content
	m_vPacket.clear();

	// copy the header into the packet
	m_vPacket.insert(m_vPacket.end(), (BYTE*) &header, ((BYTE*) &header) + sizeof(DataPacketHeader));

	// copy the projectile into the packet
	m_vPacket.insert(m_vPacket.end(), (BYTE*) &projectile, ((BYTE*) &projectile) + sizeof(Projectile));

	return;
}

const char* DataPacket::getVideoData() const
{
	return (const char*) &m_vPacket[0] + sizeof(DataPacketHeader);
}

unsigned int DataPacket::getVideoDataSize() const
{
	// grab the header from the packet
	DataPacketHeader* header = (DataPacketHeader*) &m_vPacket[0];
	return header->size;
}

cVector3d DataPacket::getPlayerPosition() const
{
	cVector3d position;

	// copy the players position from the packet
	memcpy(&position, &m_vPacket[0] + sizeof(DataPacketHeader), sizeof(cVector3d));

	return position;
}

cVector3d DataPacket::getSlingshotPosition() const
{
	cVector3d position;

	// copy the players position from the packet
	memcpy(&position, &m_vPacket[0] + sizeof(DataPacketHeader), sizeof(cVector3d));

	return position;
}

Projectile DataPacket::getProjectile() const
{
	Projectile projectile;

	// copy the projectile from the packet
	memcpy(&projectile, &m_vPacket[0] + sizeof(DataPacketHeader), sizeof(Projectile));

	return projectile;
}

const std::vector<BYTE>& DataPacket::getPacket() const
{
	return m_vPacket;
}
