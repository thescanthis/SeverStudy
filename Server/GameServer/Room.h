#pragma once
#include "Job.h"

class Room
{
public:
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	void Broadcast(SendBufferRef sendBuffer);

public:
	
	void FluhJob();

	template<typename T,typename Ret,typename... Args>
	void PushJob(Ret(T::* memFunc)(Args...), Args... args)
	{
		auto job = MakeShared<MemberJob<T, Ret, Args...>>(static_cast<T*>(this), memFunc, args...);
		_jobs.Push(job);
	}

private:
	map<uint64, PlayerRef> _players;
	JobQueue _jobs;
};

extern Room GRoom;