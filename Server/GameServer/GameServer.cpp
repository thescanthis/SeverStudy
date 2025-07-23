﻿#include "pch.h"
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
#include "Player.h"
#include <functional>

enum
{
	WORKER_TICK = 64
};

void DoWorkerJob(ServerServiceRef& service)
{
	while (true)
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;

		// 네트워크 입출력 처리 -> 인게임 로직까지 (패킷 핸들러에 의해)
		service->GetIocpCore()->Dispatch(10);

		//예약된 일감 처리
		ThreadManager::DistributeReserveJobs();

		//글로벌 큐
		ThreadManager::DoGlobalQueueWork();
	}
}
int main()
{
	GRoom->DoTimer(1000, [] {cout << "Hello 1000" << '\n'; });
	GRoom->DoTimer(2000, [] {cout << "Hello 2000" << '\n'; });
	GRoom->DoTimer(3000, [] {cout << "Hello 3000" << '\n'; });

	ClientPacketHandler::Init();

	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>, // TODO : SessionManager 등
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
			{
				DoWorkerJob(service);
			});
	}

	DoWorkerJob(service);
	GThreadManager->Join();
}