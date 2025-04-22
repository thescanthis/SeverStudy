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

void HandleError(const char* cause)
{
	int32 errorCode = ::WSAGetLastError();
	cout << cause << "ErrorCode" << errorCode << '\n';
}


int main()
{
	//Init
	WSAData wsaData;
	int err = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
		return 0;

	//정수형 포인터를 리턴
	SOCKET ServerSocket = ::socket(AF_INET, SOCK_DGRAM, 0);

	if (ServerSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777); // host to network short

	if (::bind(ServerSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)))
	{
		HandleError("Bind");
		return 0;
	}

	while (true)
	{
		SOCKADDR_IN clientAddr;
		::memset(&clientAddr, 0, sizeof(clientAddr));
		int32 addrLen = sizeof(clientAddr);

		char recvBuffer[1000];
		int32 recvLen = ::recvfrom(ServerSocket, recvBuffer,sizeof(recvBuffer),0,
			(SOCKADDR*)&clientAddr,&addrLen);

		if (recvLen <= 0)
		{
			HandleError("RecvFrom");
			return 0;
		}

		cout << "Send Data! Data =" << recvBuffer << '\n';
		cout << "Send Data! Len =" << recvLen << '\n';

		int32 errCode = ::sendto(ServerSocket, recvBuffer, recvLen, 0,
			(SOCKADDR*)&clientAddr, sizeof(clientAddr));

		if (errCode == SOCKET_ERROR)
		{
			HandleError("SendTo");
			return 0;
		}

		cout << "Send Data Len = " << recvLen << '\n';
	}

	::WSACleanup();
	return 0;
}