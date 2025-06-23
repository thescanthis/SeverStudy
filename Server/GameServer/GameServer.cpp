#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ServerPacketHandler.h"
#include "tchar.h"



int main()
{
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
		PKT_S_TEST_WRITE pktWriter(1001, 100, 10);
		PKT_S_TEST_WRITE::BuffsList buffList = pktWriter.ReserveBuffList(3);
		buffList[0] = { 100,1.5f };
		buffList[1] = { 200,2.3f };
		buffList[2] = { 300,0.7f };

		PKT_S_TEST_WRITE::BuffsVictimsList vic0 = pktWriter.ReserveBuffsVictimsList(&buffList[0], 3);
		{
			vic0[0] = 1000;
			vic0[1] = 2000;
			vic0[2] = 3000;
		}

		PKT_S_TEST_WRITE::BuffsVictimsList vic1 = pktWriter.ReserveBuffsVictimsList(&buffList[1], 1);
		{
			vic1[0] = 4000;
		}

		PKT_S_TEST_WRITE::BuffsVictimsList vic2 = pktWriter.ReserveBuffsVictimsList(&buffList[2], 2);
		{
			vic2[0] = 5000;
			vic2[1] = 6000;
		}

		SendBufferRef sendBuffer = pktWriter.CloseAndRetrun();
		GSessionManager.Broadcast(sendBuffer);
		std::this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();
}