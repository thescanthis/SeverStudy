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
//    ����			  ����
// [PKT_S_TEST][              ]
struct PKT_S_TEST
{
	struct BuffsListItem
	{
		uint64 buffId;
		float remainTime;

		//victim List
		uint16 victimsOffset;
		uint16 victimsCount;

		bool Validate(BYTE* packetStart, uint16 packetSize, OUT uint32& size)
		{
			if (victimsOffset + victimsCount * sizeof(uint64) > packetSize)
				return false;

			size += victimsCount*sizeof(uint64);
			return true;
		}
	};

	uint16 packetSize; //�������
	uint16 packetId; //�������
	uint64 id;
	uint32 hp;
	uint16 attack;
	uint16 buffsOffset;
	uint16 buffsCount;

	//���� ������
	// 1) ���ڿ�
	// 2) �׳� ����Ʈ �迭
	// 3) �Ϲ� ����Ʈ

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_TEST);
		if (packetSize < size)
			return false;

		if (buffsOffset + buffsCount * sizeof(BuffsListItem) > packetSize)
			return false;

		//Biffers ���� ������ ũ�� �߰�
		size += buffsCount * sizeof(BuffsListItem);

		BuffsList buffList = GetBuffsList();
		for (int i = 0; i < buffList.Count(); i++)
		{
			if (buffList[i].Validate((BYTE*)this, packetSize, OUT size) == false)
				return false;
		}

		//���� ũ�� ��
		if (size != packetSize)
			return false;

		return true;
	}

	using BuffsList = PacketList<PKT_S_TEST::BuffsListItem>;
	using BuffsVictimsList = PacketList<uint64>;

	BuffsList GetBuffsList()
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += buffsOffset;

		return BuffsList(reinterpret_cast<PKT_S_TEST::BuffsListItem*>(data), buffsCount);
	}

	BuffsVictimsList GetBuffsVictimsList(BuffsListItem* buffsItem)
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += buffsItem->victimsOffset;

		return BuffsVictimsList(reinterpret_cast<uint64*>(data), buffsItem->victimsCount);
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

		PKT_S_TEST::BuffsVictimsList victims =  pkt->GetBuffsVictimsList(&buffs[i]);
		cout << "Victims Count:" << victims.Count() << '\n';

		for (auto& victim : victims)
		{
			cout << "victim: " << victim << '\n';
		}
	}
}
