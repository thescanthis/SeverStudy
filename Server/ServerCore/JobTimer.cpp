#include "pch.h"
#include "JobTimer.h"
#include "JobQueue.h"

void JobTimer::Reserve(uint64 tickAfter, weak_ptr<JobQueue> owner, JobRef job)
{
	const uint64 executeTick = ::GetTickCount64() + tickAfter;
	JobData* jobData = ObjectPool<JobData>::Pop(owner, job);

	WRITE_LOCK;
	_items.push(TimerItem{ executeTick,jobData });
}

void JobTimer::Distribute(uint64 now)
{
	// �ѹ��� 1 �����常 ���
	if (_distributing.exchange(true) == false)
		return;

	Vector<TimerItem> items;
	{
		WRITE_LOCK;

		while (_items.empty() == false)
		{
			const TimerItem& timerItme = _items.top();
			if (now < timerItme.executeTick)
				break;

			items.push_back(timerItme);
			_items.pop();
		}
	}

	for (TimerItem& item : items)
	{
		if (JobQueueRef owner = item.jobData->_owner.lock())
			owner->Push(item.jobData->_job);

		ObjectPool<JobData>::Push(item.jobData);
	}

	//�������� Ǯ���ش�
	_distributing.store(false);
}

void JobTimer::Clear()
{
	WRITE_LOCK;
	while (_items.empty() == false)
	{
		const TimerItem& timerItem = _items.top();
		ObjectPool<JobData>::Push(timerItem.jobData);
		_items.pop();
	}
}