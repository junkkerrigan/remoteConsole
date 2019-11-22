#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <chrono>
#include <ctime>

using namespace std;

bool runCommands(const vector<string>& commands) {
	for (auto command : commands) {
		if (system(command.c_str()) != 0) {
			cout << "An error occured while executing the `" << command << "` command.\n\n";
			return false;
		}
		cout << "Command `" << command << "` successfully executed.\n\n";
	}
	return true;
}

int main()
{
	cout << "Server is starting...\n";

	WSADATA wsaData;
	if (WSAStartup(0x101, &wsaData) || wsaData.wVersion != 0x101) return -1;

	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == INVALID_SOCKET) return -1;

	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(2000);
	sa.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	SOCKADDR_IN nsa;
	int szNsa = sizeof(nsa);

	if (bind(s, (SOCKADDR*)&sa, sizeof(sa)) == SOCKET_ERROR) return -1;

	cout << "Server is running.\n\n";
	system("start ../Debug/Client.exe");

	vector<string> commands;
	char buffer[1024];
	int bufSz = sizeof(buffer);
	ofstream log;

	while (true) {
		if (recvfrom(s, buffer, bufSz, 0, (SOCKADDR*)&nsa, &szNsa)  
				== SOCKET_ERROR)
			return -1;
		string command(buffer);

		if (command == "") {
			string reply;
			if (commands.size() == 0) reply = "No commands were entered.";
			else if (commands.size() == 1 && commands[0] == "Who")
				reply = "Andrew Pavlenko. Variant 3: remote console.";
			else if (runCommands(commands)) reply = "Commands successfully executed.";
			else reply = "Oww... An error occured while executing ;(";
			if (sendto(s, reply.c_str(), reply.length() + 1, 0, (SOCKADDR*)&nsa, sizeof(nsa))
				== SOCKET_ERROR)
				return -1;
			commands.clear();
		}
		else {
			commands.push_back(command);
			time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
			log.open("log.txt", ios_base::out | ios_base::app);
			log << '`' << command << "` at " << ctime(&now) << '\n';
			log.close();
		}
	}

	closesocket(s);
	WSACleanup();
	system("pause");
	return 0;
}