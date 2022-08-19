#include"Server.h"

int Server::initWinsock()
{	WSAData _wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &_wsaData);
	if (iResult != 0) {
		// Some error occurd
		return iResult;
	}
	return 0;
}

int Server::initAddressInfo(addrinfo** result)
{
	addrinfo hint;
	ZeroMemory(&hint, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;
	hint.ai_flags = AI_PASSIVE;
	int iResult = getaddrinfo(NULL, _port.c_str(), &hint, result);
	if (iResult != 0) {
		WSACleanup();
		return iResult;
	}
	return 0;
}

SOCKET Server::createSocket(addrinfo * addcresInfo)
{
	SOCKET result = socket(
		addcresInfo->ai_family,
		addcresInfo->ai_socktype,
		addcresInfo->ai_protocol);
	if (result == INVALID_SOCKET) {
		freeaddrinfo(addcresInfo);
		WSACleanup();
	}

	return result;
}

void Server::workerCleaner()
{
	ConnectionWorker* worker = nullptr;
	_running = true;
	while (_running) {
		
		lock.lock(); 
		if (_workers.size() == 0) {
			lock.unlock();
			continue;
		}
		auto it_workers = _workers.begin();
		lock.unlock();
		do {
			lock.lock();
			worker = *(it_workers);
			lock.unlock();
			if (worker->getState() == DONE) {
				worker->join();
				delete worker;
				auto _it_workers = it_workers;
				lock.lock();
				it_workers = std::next(it_workers);
				_workers.erase(_it_workers);
				lock.unlock();
				if (it_workers == _workers.end())
					break;
			}
			lock.lock();
			if (std::next(it_workers) != _workers.end())
				worker = *(++it_workers);
			else
				worker = nullptr;
			lock.unlock();
		} while (worker != nullptr);
		worker = nullptr;
	}

}

void Server::intiateWorker(SOCKET soc) {
	ConnectionWorker* worker = new ConnectionWorker(soc);
	worker->start();
	lock.lock();
	_workers.push_back(worker);
	lock.unlock();
}

Server::Server(std::string port)
{
	_port = port;
	_running = false;
	_cleaner_thread = nullptr;
}
int Server::init() {
	
	int result = initWinsock();
	if (result != 0) {
		return result;
	}
	result = initAddressInfo(&_addresInfo);
	if (result != 0) {
		return result;

	}
	_sock = createSocket(_addresInfo);
	if (result != 0) {
		return result;

	}
	int iResult = bind(_sock, _addresInfo->ai_addr, (int)_addresInfo->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		return -1;
	}
	iResult = listen(_sock, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		return -1;
	}
	_cleaner_thread = new std::thread(&Server::workerCleaner, this);
	return 0;

}

int Server::nextSocket(SOCKET* soc) {
	*soc = INVALID_SOCKET;
	*soc = accept(_sock, NULL, NULL);
	if (*soc == INVALID_SOCKET) return -1;
	return 0;
}

Server::~Server() {
	_running = false;
	_cleaner_thread->join();
	freeaddrinfo(_addresInfo);
	WSACleanup();
	for (auto worker : _workers) {
		worker->join();
		delete worker;
	}
}