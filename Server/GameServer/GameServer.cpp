// GameServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <windows.h>
#include "ThreadManager.h"

class TestLock
{
	USE_LOCK;
public:
	int32 TestRead()
	{
		READ_LOCK;

		if (_queue.empty())
			return -1;

		return _queue.front();
	}

	void TestPush()
	{
		WRITE_LOCK;
		_queue.push(rand()% 100);
	}
	
	void TestPop()
	{
		WRITE_LOCK;

		if (_queue.empty() == false)
		{
			_queue.pop();
		}
	}

private:
	queue<int> _queue;
};

//지금 에러가 나기때문에 에러를 찾아함 Lock Class에서
TestLock testLock;


void ThreadWrite()
{
	while (true)
	{
		testLock.TestPush();
		this_thread::sleep_for(1ms);
		testLock.TestPop();
	}
}

void TreadRead()
{
	while (true)
	{
		int32 value = testLock.TestRead();
		cout << "Value : " << value << '\n';
		this_thread::sleep_for(1ms);
		
	}
}

int main()
{
	for (int i = 0; i < 2; i++)
	{
		GThreadManager->Launch(ThreadWrite);
	}

	for (int i = 0; i < 5; i++)
	{
		GThreadManager->Launch(TreadRead);
	}
	GThreadManager->Join();

	return 0;
}