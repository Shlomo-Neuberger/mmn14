
#include "WinSockImports.h"
#include "Common.h"
#include "Server.h"
#include "ConnectionWorker.h"


int main()
{
	std::cout << "Hello World!\n";
	Server server;
	server.init();
	do {
		SOCKET sock;
		server.nextSocket(&sock);
		server.intiateWorker(sock);
	} while (true);
	std::cout << "NOP";
}

