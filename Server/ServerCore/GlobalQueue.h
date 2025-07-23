#pragma once

/*-----------------
	GlobalQueue
-----------------*/

class GlobalQueue
{
public:
	GlobalQueue();
	~GlobalQueue();

	void Push(JobQueueRef JobQueue);
	JobQueueRef Pop();
private:
	LockQueue<JobQueueRef> _jobQueues;
};

