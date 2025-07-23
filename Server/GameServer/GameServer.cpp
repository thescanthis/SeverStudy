#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ClientPacketHandler.h"
#include "tchar.h"
#include "Protocol.pb.h"

#include "Job.h"
#include "Room.h"
#include <functional>

void HealByValue(int64 target, int32 value)
{
	cout << target << "한테 힐" << value << "만큼 줌" << '\n';
}

int main()
{


	ClientPacketHandler::Init();

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
		GRoom->FlushJob();
		this_thread::sleep_for(1ms);
	}

	GThreadManager->Join();
}