#pragma once

/*--------------------
	BaseAllocator
--------------------*/

class BaseAllocator
{
public:
	static void* Alloc(int size);
	static void Release(void* ptr);
};

/*------------------------
	  StompAllocator
메모리 오염을 방지하는 기법.
-------------------------*/

class StompAllocator
{
public:
	enum { PAGE_SIZE = 0x1000 };
public:
	static void* Alloc(int size);
	static void Release(void* ptr);
};

/*------------------------
	  PoolAllocator
-------------------------*/

class PoolAllocator
{
public:
	static void* Alloc(int size);
	static void Release(void* ptr);
};

/*------------------------
	STL StompAllocator
-------------------------*/

template<typename T>
class StlAllocator
{
public:
	using value_type = T;

	StlAllocator() {}

	template<typename Other>
	StlAllocator(const StlAllocator<Other>&) {}

	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32> (count * sizeof(T));
		return static_cast<T*>(PoolAllocator::Alloc(size));
	}

	void deallocate(T* ptr, size_t count)
	{
		PoolAllocator::Release(ptr);
	}
};