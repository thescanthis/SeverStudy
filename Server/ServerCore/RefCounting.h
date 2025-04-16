#pragma once
#include <atomic>
/*------------------
	RefCountable
------------------*/

// 최상위 객체를 하나를 만들고 RefCount 기능을 만든다.
// 상속을 받아서 구현함.
class RefCountable
{
public:
	RefCountable() :_refCount(1) {}
	virtual ~RefCountable() {}

	int GetRefCount() {return _refCount;}

	int AddRef() { return _refCount.fetch_add(1); }
	int ReleaseRef() 
	{
		int refCount = _refCount.fetch_sub(1);
		if (refCount == 0)
		{
			delete this;
		}
		return refCount;
	}
protected:

	atomic<int32> _refCount;
};


/*---------------------------
		SharedPtr
---------------------------*/

template<typename T>
class TSharedPtr
{
public:
	TSharedPtr() {}
	TSharedPtr(T* ptr) { Set(ptr); }

	//복사
	TSharedPtr(const TSharedPtr& rhs) { Set(rhs._ptr); }
	//이동
	TSharedPtr(TSharedPtr&& rhs) { _ptr = rhs._ptr; rhs._ptr = nullptr; }
	//상속관계
	template<typename U>
	TSharedPtr(const TSharedPtr<U>& rhs) { Set(static_cast<T*>(rhs._ptr)); }

	~TSharedPtr() { Release(); }

public:
	//복사 연산자
	TSharedPtr& operator=(const TSharedPtr& rhs)
	{
		if (_ptr != rhs._ptr)
		{
			Release();
			Set(rhs._ptr);
		}
		return *this;
	}

	//이동 연산자
	TSharedPtr& operator=(TSharedPtr& rhs)
	{
		Release();
		_ptr = rhs._ptr;
		rhs._ptr = nullptr;
		return *this;
	}

	bool		operator==(const TSharedPtr& rhs) const { return _ptr == rhs._ptr; }
	bool		operator==(T* ptr) const { return _ptr == ptr; }
	bool		operator!=(const TSharedPtr& rhs) const { return _ptr != rhs._ptr; }
	bool		operator!=(T* ptr) const { return _ptr != ptr; }
	bool		operator<(const TSharedPtr& rhs) const { return _ptr < rhs._ptr; }
	T*			operator*() { return _ptr; }
	const T*	operator*() const { return _ptr; }
				operator T* () const { return _ptr; }
	T*			operator->() { return _ptr; }
	const T*	operator->() const { return _ptr; }


	bool IsNull() { return _ptr == nullptr; }

public:
	inline void Set(T* ptr)
	{
		_ptr = ptr;
		if (ptr)
			ptr->AddRef();
	}

	inline void Release()
	{
		if (_ptr != nullptr)
		{
			_ptr->ReleaseRef();
			_ptr = nullptr;
		}
	}

private:
	T* _ptr = nullptr;
};