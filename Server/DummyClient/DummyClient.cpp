// DummyClient.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>

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
	SOCKET clientSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	
	if (clientSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}

	//연결할 목적지는? (IP주소 + Port == 동일해야 접근 가능.)
	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1"); 구시대//신시대
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777); // host to network short

	//Connected UDP
	::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

	while (true)
	{
		//TODO
		char sendBuffer[100] = "Hello World!";

		//Unconected UDP
		//int32 result = ::sendto(clientSocket, sendBuffer, sizeof(sendBuffer), 0,
		//	(SOCKADDR*)&serverAddr, sizeof(serverAddr));

		int32 result = ::send(clientSocket, sendBuffer, sizeof(sendBuffer),0);

		if (result == SOCKET_ERROR)
		{
			HandleError("SendTo");
			return 0;
		}
		

		cout << "Send Data! Len =" << sizeof(sendBuffer) << '\n';

		SOCKADDR_IN recvAddr;
		::memset(&recvAddr, 0, sizeof(recvAddr));
		int32 addrLen = sizeof(recvAddr);

		char recvBuffer[1000];

		//Unconected UDP
		//int32 recvLen = ::recvfrom(clientSocket, recvBuffer, sizeof(recvBuffer), 0
		//,(SOCKADDR*)&recvAddr,&addrLen);
		
		int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer),0);
		if (recvLen <= 0)
		{
			HandleError("RecvFrom");
			return 0;
		}
		
		cout << "Send Data! Data =" << recvBuffer << '\n';
		cout << "Send Data! Len =" << recvLen << '\n';

		this_thread::sleep_for(1s);
	}

	::closesocket(clientSocket);
	::WSACleanup();
}

