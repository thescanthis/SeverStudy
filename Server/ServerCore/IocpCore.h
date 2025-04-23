#pragma once

/*----------------------
	  IocpCoreObject ->Seesion
----------------------*/
class IocpObject
{
public:
	virtual HANDLE GetHandle() = 0;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) = 0;
};

/*----------------------
		IocpCore
----------------------*/


//My Handle
class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE		GetHandle() { return _iocpHandle; }

	bool		Register(class IocpObject* iocpObject);
	bool		Dispatch(uint32 timeoutMs = INFINITE); //->Check if there is a job in the internal thread

private:

	HANDLE		_iocpHandle;
};

//
extern IocpCore GIocpCore;