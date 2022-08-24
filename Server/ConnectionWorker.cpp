#include "ConnectionWorker.h"

ConnectionWorker::ConnectionWorker(SOCKET soc)
{
	_soc = soc;
	_thread = nullptr;
}

ConnectionWorker::ConnectionWorker()
{
	_soc = INVALID_SOCKET;
	_thread = nullptr;
}

time_t ConnectionWorker::getStartTime() { return startTime; }

void ConnectionWorker::run()
{
	// recv only header first
	_state = WORKING;
	startTime = time(0);
	ZeroMemory(_recvbuf, DEFUALT_BUFFER_LEN);
	int len = recv(_soc, _recvbuf, sizeof(Requests::RequestHeader) - sizeof(char *) - sizeof(UINT16), 0);
	Requests::Request req;
	req.setCommonHeader(_recvbuf, len);
	Requests::RequestBase *request = nullptr;

	switch (req._header.type)
	{
	case REQUEST_OP_PUT_FILE:
		request = new Requests::PutFileRequest(req, _soc);
		break;
	case REQUEST_OP_REMOVE_FILE:
		request = new Requests::RemoveFileRequest(req, _soc);
		break;
	case REQUEST_OP_GET_FILE:
		request = new Requests::GetFileRequest(req, _soc);
		break;
	case REQUEST_OP_LIST_FILES:
		request = new Requests::ListFilesRequest(req, _soc);
		break;
	case REQUEST_OP_UNKNOW_REQUEST:
	default:
		break;
	}

	if (request != nullptr)
	{
		request->do_request();
		delete request;
	}
	closesocket(_soc);
	_state = DONE;
}

int ConnectionWorker::join()
{
	_thread->join();
	delete _thread;
	_thread = nullptr;
	_state = IDLE;
	return 0;
}

void ConnectionWorker::start()
{
	_thread = new std::thread(&ConnectionWorker::run, this);
}

void ConnectionWorker::abort()
{
	_thread->~thread();
}

WorkerState ConnectionWorker::getState()
{
	return _state;
}
