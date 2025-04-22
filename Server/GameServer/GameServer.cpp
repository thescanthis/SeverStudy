#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

/*---------------
	SockProgram
---------------*/
#include <WinSock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")



int main()
{
	//Init
	WSAData wsaData;
	int err = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
		return 0;

	//정수형 포인터를 리턴
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);

	if (listenSocket == INVALID_SOCKET)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Socket ErrorCode" << errCode << '\n';
		return 0;
	}

	//나의주소는? (IP주소 + Port)
	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY); // 니가 알아서 골라줘.
	serverAddr.sin_port = ::htons(7777); // host to network short

	//안내원 폰 개통
	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)))
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Bind ErrorCode" << '\n';
		return 0;
	}

	//영업시작 Socket,최대 queue에 쌓이는 크기 그이상 들어온 요청은 거절,무시
	if (::listen(listenSocket, 10))
	{
		int32 errCode = ::WSAGetLastError();
		cout << "listen ErrorCode" << '\n';
		return 0;
	}

	while (true)
	{
		SOCKADDR_IN clientAddr;
		::memset(&clientAddr, 0, sizeof(clientAddr));

		int32 addrLen = sizeof(clientAddr);
		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
	
		if (clientSocket == INVALID_SOCKET)
		{
			int32 errorCode = ::WSAGetLastError();
			cout << "clientSocket" << '\n';
			return 0;
		}

		//손님 입장!
		char ipAddress[16];
		::inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress, sizeof(ipAddress));
		cout << "Client Connected IP = " << ipAddress << '\n';

		//TODO

		while (true)
		{
			char recvBuffer[1000];
			this_thread::sleep_for(1s);

			int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);

			if (recvLen <=0)
			{
				int32 errorCode = ::WSAGetLastError();
				cout << "Recv_ERROR:" << errorCode << '\n';
				return 0;
			}

			cout << "Send Data! Data =" << recvBuffer << '\n';
			cout << "Send Data! Len =" << recvLen << '\n';

			//int32 result = ::send(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			//
			//if (result == SOCKET_ERROR)
			//{
			//	int32 errorCode = ::WSAGetLastError();
			//	cout << "SOCKET_ERROR:" << errorCode << '\n';
			//	return 0;
			//}

		}
	}
	return 0;
}