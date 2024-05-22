#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

int main()
{
    WSADATA wsaData;
    ADDRINFO  hints;
    ADDRINFO* addrResult;
    SOCKET ConnectSocket = INVALID_SOCKET;
    SOCKET ListenSocket = INVALID_SOCKET;//слушает соединение
    char recvBuffer[512];

    const char* sendBuffer = "Hello from server";

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);//вса - виндовск соект апи. с помощью нее инициализируем библиотеки
    if (result != 0) {
        cout << "WSAStartup failed result" << endl;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints)); //обнуление
    hints.ai_family = AF_INET; //семейство
    hints.ai_socktype = SOCK_STREAM; //тип сокета
    hints.ai_protocol = IPPROTO_TCP; //протокол
    hints.ai_flags = AI_PASSIVE; //сервер - пассивный клиент - активный

    result = getaddrinfo(NULL, "666", &hints, &addrResult); //инфа о сетевом адресе и порта
    if (result != 0) {
        cout << "getaddrinfo failed with error" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    ListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        cout << "Socket error" << endl;
        freeaddrinfo(addrResult);
        return 1;
    }

    result = bind(ListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
    if (result == SOCKET_ERROR) {
        cout << "Unable connect to server" << endl;
        closesocket(ListenSocket);
        ListenSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    result = listen(ListenSocket, SOMAXCONN); //макс колво соеди
    if (result == SOCKET_ERROR) {
        cout << "listening socket failed" << result << endl;
        closesocket(ListenSocket);
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    ConnectSocket = accept(ListenSocket, NULL, NULL);
    if (ConnectSocket == SOCKET_ERROR) {
        cout << "listening socket failed" << result << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }





    do {
        ZeroMemory(recvBuffer, 512);
        result = recv(ConnectSocket, recvBuffer, 512, 0);

        if (result > 0) {
            cout << "Received" << result << "bytes" << endl;
            cout << "Received data" << recvBuffer << endl;

            result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);

            if (result == SOCKET_ERROR) {
                cout << "Send failed, error" << result << endl;
                closesocket(ListenSocket);
                freeaddrinfo(addrResult);
                WSACleanup();
                return 1;
            }
        }
        else if (result == 0) {
            cout << "Connection closing" << endl;
        }
        else
        {
            cout << "Received failed" << endl;
            closesocket(ConnectSocket);
        }


    } while (result > 0);
    result = shutdown(ConnectSocket, SD_SEND);

    if (result == SOCKET_ERROR) {
        cout << "shutdown failed, error" << result << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(addrResult);
    WSACleanup();
    return 0;


}