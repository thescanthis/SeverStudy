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
#include "RefCounting.h"

class Wraight : public RefCountable
{
public:
	int _hp = 150;
	int _posX = 0;
	int _posY = 0;
};

using WarightRef = TSharedPtr<Wraight>;

class Missile : public RefCountable
{
public:
	void SetTarget(WarightRef target)
	{
		_target = target;
	}

	void Update()
	{
		if (_target == nullptr)
			return;
		int posX = _target->_posX;
		int posY = _target->_posY;

		//if (_target->_hp == 0)
		//{
		//	_target->ReleaseRef();
		//	_target = nullptr;
		//}
		_target->_hp -= 10;
	}

	WarightRef _target = nullptr;
};


using MissileRef = TSharedPtr<Missile>;

int main()
{
	WarightRef wraight(new Wraight());
	wraight->ReleaseRef();

	MissileRef missile(new Missile());
	missile->ReleaseRef();

	wraight = nullptr;


	while (true)
	{
		missile->Update();
		if (missile->_target->_hp == 0)
		{
			missile->_target->ReleaseRef();
			missile->_target = nullptr;
			missile = nullptr;
			break;
		}
	}
	return 0;
}