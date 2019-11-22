#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;


int main(int argc, char* argv[])
{
	WSADATA wsaData;
	if (WSAStartup(0x101, &wsaData) || wsaData.wVersion != 0x101) return -1;

	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == INVALID_SOCKET) return -1;

	SOCKADDR_IN sa;
	int szSa = sizeof(sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(2000);
	sa.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	SOCKADDR_IN nsa;
	int szNsa = sizeof(nsa);

	string inviteMsg = "Enter commands. To finish entering, just enter empty string. To quit, press Ctrl+C.";
	cout << inviteMsg << "\n";
	string command;
	char buffer[1024];
	int bufSz = sizeof(buffer);

	while (true) {
		getline(cin, command);
		if (sendto(s, command.c_str(), command.length() + 1, 0, (SOCKADDR*)&sa, sizeof(sa)) 
			== SOCKET_ERROR)
			return -1;

		if (command == "") {
			if (recvfrom(s, buffer, bufSz, 0, (SOCKADDR*)&sa, &szSa)
				== SOCKET_ERROR)
				return -1;
			cout << buffer << "\n\n";
			cout << inviteMsg << "\n";
		}
	}

	closesocket(s);
	WSACleanup();
	system("pause");
	return 0;
}