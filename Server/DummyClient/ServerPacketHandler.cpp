#include "pch.h"
#include "ServerPacketHandler.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	//TODO Log
	return true;
}

bool Handle_S_TEST(PacketSessionRef& session, Protocol::S_TEST& pkt)
{
	cout << pkt.id() << " " << pkt.hp() << " " << pkt.attack() << '\n';

	cout << "BUFSIZE: " << pkt.buffs_size() << '\n';

	for (auto& buf : pkt.buffs())
	{
		cout << "BUFINFO : " << buf.buffid() << " " << buf.remaintime() << '\n';
		cout << "VICTIMS : " << buf.victims_size() << '\n';

		for (auto& vic : buf.victims())
		{
			cout << vic << '\n';
		}
	}
	
	return true;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	return false;
}
