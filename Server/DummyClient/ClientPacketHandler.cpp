#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"

void ClientPacketHandler::HandlePacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	switch (header.id)
	{
	case S_TEST:
		Handle_S_TEST(buffer,len);
		break;
	}
}

#pragma pack(1)
//    고정			  가변
// [PKT_S_TEST][              ]
struct PKT_S_TEST
{
	struct BuffListItem
	{
		uint64 buffId;
		float remainTime;
	};

	uint16 packetSize; //공용헤더
	uint16 packetId; //공용헤더
	uint64 id;
	uint32 hp;
	uint16 attack;
	uint16 buffsOffset;
	uint16 buffsCount;

	//가변 데이터
	// 1) 문자열
	// 2) 그냥 바이트 배열
	// 3) 일반 리스트

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_TEST);
		if (packetSize < size)
			return false;

		size += buffsCount * sizeof(BuffListItem);
		if (size != packetSize)
			return false;

		if (buffsOffset + buffsCount * sizeof(BuffListItem) > packetSize)
			return false;

		return true;
	}

	using BuffsList = PacketList<PKT_S_TEST::BuffListItem>;

	BuffsList GetBuffsList()
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += buffsOffset;

		return BuffsList(reinterpret_cast<PKT_S_TEST::BuffListItem*>(data), buffsCount);
	}
};
#pragma pack()
//[][															]
void ClientPacketHandler::Handle_S_TEST(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PKT_S_TEST* pkt = reinterpret_cast<PKT_S_TEST*>(buffer);

	if (pkt->Validate() == false)
		return;

	PKT_S_TEST::BuffsList buffs = pkt->GetBuffsList();

	cout << "BufCount: " << buffs.Count()<<'\n';
	for (int32 i = 0; i < buffs.Count(); i++)
	{
		cout << "BufInfo: " << buffs[i].buffId <<" "<< buffs[i].remainTime << '\n';
	}

	for (auto it = buffs.begin(); it != buffs.end(); ++it)
	{
		cout << "BufInfo : " << it->buffId << " " << it->remainTime << '\n';
	}

	for (auto& buff : buffs)
	{
		cout << "BufInfo : " << buff.buffId << " " << buff.remainTime << '\n';
	}
}
