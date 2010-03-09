// ChatPacket.h: interface for the CChatPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATPACKET_H__0AABB4AC_CB11_405B_A32C_FE8238F1B6F9__INCLUDED_)
#define AFX_CHATPACKET_H__0AABB4AC_CB11_405B_A32C_FE8238F1B6F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CChatPacket  
{
public:
	CChatPacket();
	virtual ~CChatPacket();

	enum PacketType {
		PACKET_DISCONNET= 0,
		PACKET_CHAT,
		PACKET_NAME,
		PACKET_UDP_PORT,
		PACKET_CONFIG,
		PACKET_FORCE_DWORD
	};

	void setType(PacketType type);
	PacketType getType();

	BYTE* getPacketPtr();
	DWORD getPacketSize();

	bool readPacket(vector<BYTE>& input);

	void writeChar(char c);
	void writeInt(int i);
	void writeString(CString str);
	void writeByteArray(BYTE* p, UINT len);

private:
	PacketType m_type;
	vector<BYTE> m_vPacket;
};

#endif // !defined(AFX_CHATPACKET_H__0AABB4AC_CB11_405B_A32C_FE8238F1B6F9__INCLUDED_)
