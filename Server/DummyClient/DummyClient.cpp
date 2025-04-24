﻿#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"

char sendBuffer[] = "Hello World";

class ServerSession : public Session
{
public:
	~ServerSession()
	{
		cout << "~ServerSeesion" << '\n';
	}
	virtual void OnConnected() override
	{
		cout << "Connected To Server" << '\n';
		Send((BYTE*)sendBuffer, sizeof(sendBuffer));

	}

	virtual int32 OnRecv(BYTE* buffer, int32 len) override
	{
		//Echo
		cout << "OnRecv Len = " << len << '\n';

		this_thread::sleep_for(1s);
		Send((BYTE*)sendBuffer, sizeof(sendBuffer));
		return len;
	}

	virtual void OnSend(int32 len) override
	{
		cout << "OnSend Len = " << len << '\n';
	}

	virtual void OnDisconnected() override
	{
		cout << "Disconnected" << '\n';
	}
};

int main()
{
	this_thread::sleep_for(1s);

	ClientServiceRef service = MakeShared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<ServerSession>, // TODO : SessionManager 등
		1);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 2; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	GThreadManager->Join();
}