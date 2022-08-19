#pragma once
#include <thread>
#include "WinSockImports.h"
#include "Common.h"
#include "RequestBase.h"
#include "PutFileRequest.h"
#include "RemoveFileRequest.h"
enum WorkerState
{
	IDLE,
	WORKING,
	DONE

};

class ConnectionWorker {
private:
	SOCKET _soc;
	std::thread* _thread;
	char _recvbuf[DEFUALT_BUFFER_LEN] = { 0 };
	WorkerState _state = IDLE;
public:
	ConnectionWorker(SOCKET soc);
	ConnectionWorker();
	void run();
	int join();
	void start();
	WorkerState getState();

};

