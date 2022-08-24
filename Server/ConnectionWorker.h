#pragma once
#include <thread>
#include "WinSockImports.h"
#include "Common.h"
#include "RequestBase.h"
#include "PutFileRequest.h"
#include "RemoveFileRequest.h"
#include "ListFilesRequest.h"
#include "GetFileRequest.h"
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
	time_t startTime=0;
public:
	ConnectionWorker(SOCKET soc);
	ConnectionWorker();
	void run();
	int join();
	void start();
	void abort();
	time_t getStartTime();
	WorkerState getState();

};

