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
	// start us off with an empty packet
	m_header.isVideo = false;
	m_header.isTactile = false;
	m_header.ARDataSize = 0;
	m_header.DepthSize = 0;
	m_header.ImageSize = 0;
	m_header.TactDataSize = 0;
}

DataPacket::~DataPacket()
{
	// nothing to delete
}

const DataPacketHeader& DataPacket::getHeader() const
{
	return m_header;
}

const std::vector<BYTE>& DataPacket::getVideoRGBData() const
{
	return m_vVideoRGBData;
}

const std::vector<BYTE>& DataPacket::getVideoDepthData() const
{
	return m_vVideoDepthData;
}

const std::vector<BYTE>& DataPacket::getVideoARData() const
{
	return m_vVideoARData;
}

const std::vector<BYTE>& DataPacket::getTactileData() const
{
	return m_vTactileData;
}

bool DataPacket::readPacket(std::vector<BYTE>& input)
{
	if (input.size() < sizeof(DataPacketHeader))
	{
		// all data packets need to have a least a header
		return false;
	}

	// grab the header from the packet
	DataPacketHeader tmpHeader;
	memcpy(&tmpHeader, &input[0], sizeof(DataPacketHeader));

	// calculate the size of the packet
	size_t size = sizeof(DataPacketHeader);
	if (tmpHeader.isVideo)
	{
		size += tmpHeader.ImageSize + tmpHeader.DepthSize + tmpHeader.ARDataSize;
	}
	if (tmpHeader.isTactile)
	{
		size += tmpHeader.TactDataSize;
	}

	// the stream needs to be bigger or equal to the packet size for the packet to be complete
	if (size <= input.size())
	{
		// we will use this counter to navigate through the stream of bytes
		unsigned int dataIndex = 0;

		// copy the header over
		m_header = tmpHeader;
		dataIndex += sizeof(DataPacketHeader);

		// clear all the sections of the packet
		m_vVideoRGBData.clear();
		m_vVideoDepthData.clear();
		m_vVideoARData.clear();
		m_vTactileData.clear();

		if (m_header.isVideo)
		{
			// add the rgb data
			m_vVideoRGBData.insert(m_vVideoRGBData.end(), &input[dataIndex], &input[dataIndex] + m_header.ImageSize);
			dataIndex += m_header.ImageSize;

			// add the depth data
			m_vVideoDepthData.insert(m_vVideoDepthData.end(), &input[dataIndex], &input[dataIndex] + m_header.DepthSize);
			dataIndex += m_header.DepthSize;

			// add the ar data
			m_vVideoARData.insert(m_vVideoARData.end(), &input[dataIndex], &input[dataIndex] + m_header.ARDataSize);
			dataIndex += m_header.ARDataSize;
		}

		if (m_header.isTactile)
		{
			// add the tactile data
			m_vTactileData.insert(m_vTactileData.end(), &input[dataIndex], &input[dataIndex] + m_header.TactDataSize);
			dataIndex += m_header.TactDataSize;
		}

		// erase this packet from the input stream
		input.erase(input.begin(), input.begin() + size);

		return true;
	}
	return false;
}

void DataPacket::setVideoData(const std::vector<BYTE>& vRGB, const std::vector<BYTE>& vDepth, const std::vector<BYTE>& vARData)
{
	m_header.isVideo = true;
	m_header.ImageSize = vRGB.size();
	m_header.DepthSize = vDepth.size();
	m_header.ARDataSize = vARData.size();
	m_vVideoRGBData = vRGB;
	m_vVideoDepthData = vDepth;
	m_vVideoARData = vARData;
	return;
}

void DataPacket::setTactileData(const std::vector<BYTE>& vTactileData)
{
	m_header.isTactile = true;
	m_header.TactDataSize = vTactileData.size();
	m_vTactileData = vTactileData;
	return;
}

std::auto_ptr<std::vector<BYTE> > DataPacket::getPacket() const
{
	std::auto_ptr<std::vector<BYTE> > packet = std::auto_ptr<std::vector<BYTE> >(new std::vector<BYTE>());

	// add the header
	packet->insert(packet->end(), (BYTE*) &m_header, (BYTE*) &m_header + sizeof(DataPacketHeader));

	if (m_header.isVideo)
	{
		// add the video rgb data
		packet->insert(packet->end(), &m_vVideoRGBData[0], &m_vVideoRGBData[0] + m_vVideoRGBData.size());

		// add the video depth data
		packet->insert(packet->end(), &m_vVideoDepthData[0], &m_vVideoDepthData[0] + m_vVideoDepthData.size());

		// add the video AR data
		packet->insert(packet->end(), &m_vVideoARData[0], &m_vVideoARData[0] + m_vVideoARData.size());
	}

	if (m_header.isTactile)
	{
		// add the tactile data
		packet->insert(packet->end(), &m_vTactileData[0], &m_vTactileData[0] + m_vTactileData.size());
	}

	return packet;
}
