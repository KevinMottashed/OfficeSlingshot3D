// ChatPacket.cpp: implementation of the CChatPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Chat.h"
#include "ChatPacket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatPacket::CChatPacket()
{

}

CChatPacket::~CChatPacket()
{

}

void CChatPacket::setType(PacketType type)
{
	m_type = type;
}

CChatPacket::PacketType CChatPacket::getType()
{
	return m_type;
}

BYTE* CChatPacket::getPacketPtr()
{
	return &m_vPacket[0];
}

DWORD CChatPacket::getPacketSize()
{
	return (DWORD) m_vPacket.size();
}

bool CChatPacket::readPacket(vector<BYTE>& input)
{
	if (input.size() < 8) {
		return false;
	}
	DWORD size = *(DWORD*) &input[0];
	PacketType type = *(PacketType*) &input[4];
	if (size <= input.size()) {
		setType(type);
		m_vPacket.clear();
		m_vPacket.insert(m_vPacket.end(), input.begin() + 8, input.begin() + size);
		input.erase(input.begin(), input.begin() + size);
		return true;
	} else {
		return false;
	}
}

void CChatPacket::writeChar(char c)
{
	m_vPacket.push_back((BYTE) c);
}

void CChatPacket::writeInt(int i)
{
/*	BYTE* p = (BYTE*) &i;
	for (int n = 0; n < 4; n++) {
		m_vPacket.push_back(p[n]);
	} */
	m_vPacket.insert(m_vPacket.end(), (BYTE*) &i, (BYTE*) (&i + 1));
}

void CChatPacket::writeString(CString str)
{
/*	for (int i = 0; i < str.GetLength(); i++) {
		m_vPacket.push_back(str[i]);
	} */
	m_vPacket.insert(m_vPacket.end(), (BYTE*) (LPCSTR) str, (BYTE*) (LPCSTR) str + str.GetLength());
}

void CChatPacket::writeByteArray(BYTE* p, UINT len)
{
	m_vPacket.resize(m_vPacket.size() + len);
	BYTE* dest = &m_vPacket[m_vPacket.size() - len];
	memcpy(dest, p, len);
}
