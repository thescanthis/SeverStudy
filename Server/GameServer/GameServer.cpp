﻿#include "pch.h"
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
		vector<BuffData> buffs{ BuffData{100,1.5f},BuffData{200,2.3f},BuffData{300,0.7f} };
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_TEST(1001, 100, 10, buffs,L"안녕하세요");
		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();
}