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

class ConnectionWorker
{
private:
	// socket that the worker uses
	SOCKET _soc;
	// thread that the worker uses
	std::thread *_thread;
	// the recv buffer the worker uses
	char _recvbuf[DEFUALT_BUFFER_LEN] = {0};
	// state flag for managing the worker
	WorkerState _state = IDLE;
	// indecates when worker start working
	time_t startTime = 0;
	/**
	 * @brief The worker job that need to run
	 *
	 */
	void run();

public:
	/**
	 * @brief Construct a new Connection Worker object
	 *
	 * @param soc that the worker will use
	 */
	ConnectionWorker(SOCKET soc);
	/**
	 * @brief Construct a new Connection Worker object
	 *
	 */
	ConnectionWorker();
	/**
	 * @brief Joins to the worker thread
	 *
	 * @return int the result of the worker
	 */
	int join();
	/**
	 * @brief Start the worker run
	 *
	 */
	void start();
	/**
	 * @brief Tries to abort the thread gracfully
	 *
	 */
	void abort();
	/**
	 * @brief Get the Start Time of the worker
	 *
	 * @return time_t
	 */
	time_t getStartTime();
	/**
	 * @brief Get the State of the worker
	 *
	 * @return WorkerState
	 */
	WorkerState getState();
};
