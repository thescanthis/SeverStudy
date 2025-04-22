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

int main()
{
	//Init
	WSAData wsaData;
	int err = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
		return 0;

	//정수형 포인터를 리턴
	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	
	if (clientSocket == INVALID_SOCKET)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Socket ErrorCode" << errCode << '\n';
		return 0;
	}

	//연결할 목적지는? (IP주소 + Port == 동일해야 접근 가능.)
	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1"); 구시대//신시대
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777); // host to network short
	
	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)))
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Connect ErrorCode" << errCode << '\n';
		return 0;
	}

	//연결 성공
	cout << "Connected To Server"<<'\n';

	while (true)
	{
		//TODO
		this_thread::sleep_for(1s);
	}

	::closesocket(clientSocket);
	::WSACleanup();
}

