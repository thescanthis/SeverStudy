#pragma once
#include "Job.h"
#include "JobQueue.h"

class Room : public JobQueue
{
public:
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	void Broadcast(SendBufferRef sendBuffer);

private:
	map<uint64, PlayerRef> _players;
};

extern std::shared_ptr<Room> GRoom;