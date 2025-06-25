#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ServerPacketHandler.h"
#include "tchar.h"
#include "Protocol.pb.h"


int main()
{
	ServerPacketHandler::Init();

	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>, // TODO : SessionManager 등
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	char sendData[1000] = "가"; // CP949 = KS-X-1001 (한글2바이트),(로마1바이트)
	char sendData1[1000] = u8"가";// UTF8 = Unicode (한글3바이트),(로마1바이트)
	WCHAR sendData2[1000] = L"가";// UTF16 = Unicode(한글2바이트),(로마2바이트) 
	TCHAR sendData3[1000] = L"가"; // tchar.h(속성에따라 달라지는 데이터셋)

	while (true)
	{
		Protocol::S_TEST pkt;
		pkt.set_id(1000);
		pkt.set_hp(100);
		pkt.set_attack(10);

		{
			Protocol::BuffData* data = pkt.add_buffs();
			data->set_buffid(100);
			data->set_remaintime(1.2f);
			data->add_victims(4000);
		}

		{
			Protocol::BuffData* data = pkt.add_buffs();
			data->set_buffid(200);
			data->set_remaintime(2.2f);
			data->add_victims(1000);
			data->add_victims(2000);
		}

		 SendBufferRef sendBuffer =  ServerPacketHandler::MakeSendBuffer(pkt);
		 GSessionManager.Broadcast(sendBuffer);

		std::this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();
}