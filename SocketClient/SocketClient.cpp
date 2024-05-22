#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
using namespace std;

int main() {

	WSAData wsaData;
	ADDRINFO hints;
	ADDRINFO* addResult;
	SOCKET ConnectSocket = INVALID_SOCKET;
	char recvBuffer[512];

	const char* sendBuffor1 = "Hello from Client:";
	const char* sendBuffor2 = "This is the second message from Client.";

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		cout << "WSAStartup failed result" << endl;
		return 1;
	}
	ZeroMemory(&hints, sizeof(hints)); //ZeroMemory используется для обнуления
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;


	getaddrinfo("localhost", "666", &hints, &addResult);
	if (result != 0) {
		cout << "getaddrinfo failed with error" << endl;
		WSACleanup();
		return 1;
	}

	ConnectSocket = socket(addResult->ai_family, addResult->ai_socktype, addResult->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET) {
		cout << "Soacet creation with " << endl;
		WSACleanup();
		return 1;
	}

	result = connect(ConnectSocket, addResult->ai_addr, (int)addResult->ai_addrlen);
	if (result == SOCKET_ERROR) {
		cout << "Unable conect to server" << endl;
		closesocket(ConnectSocket);
		ConnectSocket == INVALID_SOCKET;
		freeaddrinfo(addResult);
		WSACleanup();
		return 1;
	}
	result = send(ConnectSocket, sendBuffor1, (int)strlen(sendBuffor1), 0);
	if (result == SOCKET_ERROR) {
		cout << "Send failed errror" << endl;
		freeaddrinfo(addResult);
		WSACleanup();
		return 1;
	}
	cout << "Send:" << result << "bytes:" << endl;

	result = send(ConnectSocket, sendBuffor2, (int)strlen(sendBuffor2), 0);
	if (result == SOCKET_ERROR) {
		cout << "Send failed errror" << endl;
		freeaddrinfo(addResult);
		WSACleanup();
		return 1;
	}
	cout << "Send:" << result << "bytes:" << endl;

	result = shutdown(ConnectSocket, SD_SEND);
	if (result == SOCKET_ERROR) {
		cout << "shutdown failed errror" << endl;
		freeaddrinfo(addResult);
		WSACleanup();
		return 1;
	}

	do {
		ZeroMemory(recvBuffer, 512);
		result = recv(ConnectSocket, recvBuffer, 512, 0);
		if (result == SOCKET_ERROR) {
			cout << "Recevied:" << result << "bytes:" << endl;
			cout << "Recevied data:" << result << "bytes:" << endl;

		}
		else if (result == 0)

			cout << "Recevied:" << result << "bytes:" << endl;


	} while (result > 0);
	freeaddrinfo(addResult);
	WSACleanup();
	return 0;

}
