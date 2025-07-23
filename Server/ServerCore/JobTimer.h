#pragma once

struct JobData
{
	JobData(weak_ptr<JobQueue> owner, JobRef job) : _owner(owner), _job(job)
	{

	}
	weak_ptr<JobQueue> _owner;
	JobRef			   _job;
};

struct TimerItem
{
	bool operator<(const TimerItem& other) const
	{
		return executeTick > other.executeTick;
	}

	uint64 executeTick = 0;
	JobData* jobData = nullptr;
};

/*-------------
	JobTimer
-------------*/

class JobTimer
{
public:
	void Reserve(uint64 tickAfter, weak_ptr<JobQueue> owner, JobRef job);
	void Distribute(uint64 now);
	void Clear();
private:
	USE_LOCK;
	PriorityQueue<TimerItem> _items;
	Atomic<bool>	_distributing = false;
};

