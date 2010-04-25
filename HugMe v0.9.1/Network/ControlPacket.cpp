#include "ControlPacket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ControlPacket::ControlPacket()
{
	// start off with an unknown packet of size 0
	ControlPacketHeader header;
	header.size = 0;
	header.type = CONTROL_PACKET_UNKNOWN;
	m_vPacket.insert(m_vPacket.end(), (BYTE*) &header, ((BYTE*) &header) + sizeof(ControlPacketHeader));
}

ControlPacket::~ControlPacket()
{
}

void ControlPacket::setChatMessage(const std::string& message)
{
	ControlPacketHeader header;
	header.size = message.size();
	header.type = CONTROL_PACKET_CHAT;
	writeStringPacket(header, message);
	return;
}

void ControlPacket::setUserName(const std::string& name)
{
	ControlPacketHeader header;
	header.size = name.size();
	header.type = CONTROL_PACKET_NAME;
	writeStringPacket(header, name);
	return;
}

void ControlPacket::setStartGame()
{
	ControlPacketHeader header;
	header.size = 0; // no data in a start game packet
	header.type = CONTROL_PACKET_START_GAME;
	writeEmptyPacket(header);
	return;
}

void ControlPacket::setPauseGame()
{
	ControlPacketHeader header;
	header.size = 0; // no data in a start game packet
	header.type = CONTROL_PACKET_PAUSE_GAME;
	writeEmptyPacket(header);
	return;
}

void ControlPacket::setEndGame()
{
	ControlPacketHeader header;
	header.size = 0; // no data in an end game packet
	header.type = CONTROL_PACKET_END_GAME;
	writeEmptyPacket(header);
	return;
}

const std::vector<BYTE>& ControlPacket::getPacket() const
{
	return m_vPacket;
}

void ControlPacket::writeStringPacket(ControlPacketHeader header, const std::string& data)
{
	// we need 1 extra character for the null character
	header.size += 1;

	// clear the existing content
	m_vPacket.clear();

	// copy the header into the packet
	m_vPacket.insert(m_vPacket.end(), (BYTE*) &header, ((BYTE*) &header) + sizeof(ControlPacketHeader));

	// copy the string into the packet
	std::string::const_iterator it = data.begin();
	for (; it != data.end(); ++it)
	{
		m_vPacket.push_back((BYTE) *it);
	}

	// all strings must be null terminated
	m_vPacket.push_back('\0');

	return;
}

void ControlPacket::writeEmptyPacket(ControlPacketHeader header)
{
	// clear the existing content
	m_vPacket.clear();

	// copy the header into the packet
	m_vPacket.insert(m_vPacket.end(), (BYTE*) &header, ((BYTE*) &header) + sizeof(ControlPacketHeader));

	return;
}

bool ControlPacket::readPacket(std::vector<BYTE>& input)
{
	if (input.size() < sizeof(ControlPacketHeader))
	{
		// all packets need to have a least a header
		return false;
	}

	// grab the header from the packet
	ControlPacketHeader* header = (ControlPacketHeader*) &input[0];

	// calculate the size of the packet
	unsigned int size = sizeof(ControlPacketHeader) + header->size;

	// the stream needs to be bigger or equal to the packet size for the packet to be complete
	if (size <= input.size())
	{
		// we will use this counter to navigate through the stream of bytes
		unsigned int dataIndex = 0;

		// clear the packet
		m_vPacket.clear();

		// add the header to the packet
		m_vPacket.insert(m_vPacket.end(), &input[dataIndex], &input[dataIndex] + sizeof(ControlPacketHeader));
		dataIndex += sizeof(ControlPacketHeader);

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

std::string ControlPacket::getUserName() const
{
	std::string str = (const char*) &m_vPacket[sizeof(ControlPacketHeader)];
	return str;
}

std::string ControlPacket::getChatMessage() const
{
	std::string str = (const char*) &m_vPacket[sizeof(ControlPacketHeader)];
	return str;
}

ControlPacketType ControlPacket::getType() const
{
	// grab the header from the packet
	ControlPacketHeader* header = (ControlPacketHeader*) &m_vPacket[0];
	return header->type;
}

