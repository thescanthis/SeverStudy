#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"

/*--------------
	Session
---------------*/
class Service;

class Session : public IocpObject
{
	friend class Listener;
	friend class IocpCore;
	friend class Service;
public:
	Session();
	virtual ~Session();

public:
						/*�ܺο��� ���*/
	void				Send(BYTE* buffer, int32 len);
	bool				Connects();
	void				Disconnect(const WCHAR* cause);

	shared_ptr<Service> GetService() { return _service.lock(); }
	void				SetService(shared_ptr<Service> service) {  _service = service; }

public:
						/* ���� ���� */
	void				SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress			GetAddress() { return _netAddress; }
	SOCKET				GetSocket() { return _socket; }
	bool				IsConnected() { return _connected; }
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }


private:
						/* �������̽� ���� */
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
						/*���� ����*/
	bool				RegisterConnect();
	bool				RegisterDisConnect();
	void				RegisterRecv();
	void				RegisterSend(SendEvent* sendEvent);

	void				ProcessConnect();
	void				ProcessDisConnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(SendEvent* sendEvent, int32 numOfBytes);
	
	void				HandleError(int32 errorCode);

protected:
						/* ������ �ڵ忡�� ������ */
	virtual void		OnConnected() {}
	virtual int32		OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void		OnSend(int32 len) {}
	virtual void		OnDisconnected() {}

public:
	// TEMP
	BYTE _recvBuffer[1000];

	// Circular Buffer[     ] -> �������� ����.
	//char _sendBuffer[1000];
	//int32 _sendLen;

private:
	weak_ptr<Service>	_service;
	SOCKET				_socket = INVALID_SOCKET;
	NetAddress			_netAddress = {};
	Atomic<bool>		_connected = false;

private:
	USE_LOCK;
	/* ���� ���� */

	/* �۽� ���� */

private:

						/*IocpEvent ����*/
	ConnectEvent		_connectEvent;
	DisConnectEvent		_disconnectEvent;
	RecvEvent			_recvEvent;
};

