#include "pch.h"
#include "memoryPool.h"

MemoryPool::MemoryPool(int32 allocSize) : _allocSize(allocSize)
{
	::InitializeSListHead(&_header);
}

MemoryPool::~MemoryPool()
{
	while (MemoryHeader* Memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header)))
		::_aligned_free(Memory);
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	ptr->allocSize = 0;
	//첫번째 주소를 바꿔줌 _header값을 바꿔주는거임 
	::InterlockedPushEntrySList(&_header, static_cast<PSLIST_ENTRY>(ptr));

	_useCount.fetch_sub(1);
	_ReserveCount.fetch_add(1);
}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* Memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));

	//없으면 할당
	if (Memory == nullptr)
	{
		Memory = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(_allocSize, SLIST_ALIGNMENT));
	}
	else {
		ASSERT_CRASH(Memory->allocSize == 0);
		_ReserveCount.fetch_sub(1);
	}

	_useCount.fetch_add(1);
	return Memory;
}
