#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"
#include "DeadLockProfiler.h"

void Lock::WriteLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif

	// ������ �����尡 ���� �����ϰ� �ִٸ� ������ ����!
	// �ƹ��� ���� �� �����ϰ� ���� ���� �� , �����ؼ� �������� ��´�.
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK >> 16);

	if (LThreadId == lockThreadId)
	{
		_wirteCount++;
		return;
	}

	const int64 beginTick = ::GetTickCount64();
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(OUT expected, desired))
			{
				_wirteCount++;
				return;
			}
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");
		this_thread::yield();
	}


}

void Lock::WriteUnLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif
	//ReadLock �� Ǯ�� ������ WriteUnLock�� �Ұ���.
	if ((_lockFlag.load() & READ_COUNT_MASK) != 0)
		CRASH("INVALID_UNLCOK_ORDER");

	const int32 lockCount = --_wirteCount;
	if (lockCount == 0)
	{
		_lockFlag.store(EMPTY_FLAG);
	}
}

void Lock::ReadLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif
	//������ �����尡 �����ϰ� �ִٸ� ������ ����
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK >> 16);

	if (LThreadId == lockThreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	// �ƹ��� ���� ���� �ϰ� ���� ���� �� �����ؼ� ���� ī��Ʈ�� �ø���.

	const int64 beginTick = ::GetTickCount64();
	while (true)
	{
		for (int spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			//[WWWWWWWW] [WWWWWWWW] Write_Count_MASK�� 0���� �� �о��ִ� BitFlag ����
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);

			// 1. �̹� �ٸ� �����忡�� Wirte�� ��� �ִ�.
			// 2. ���� �̻��̿� ������ ��ġ�⸦ �ؼ� ���� �ڵ带 �����ߴ�.
			if (_lockFlag.compare_exchange_strong(expected, expected + 1))
				return;
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::ReadUnLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif

	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		CRASH("MULTIPLE_UNLOCK");
}
