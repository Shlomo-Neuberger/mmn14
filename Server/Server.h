#pragma once
#include <list>
#include <iterator>
#include <thread>
#include <mutex>
#include "WinSockImports.h"
#include "Common.h"
#include "ConnectionWorker.h"
class Server
{
private:
	std::string _port;
	addrinfo *_addresInfo;
	SOCKET _sock;
	std::list<ConnectionWorker *> _workers;
	bool _running;
	std::thread *_cleaner_thread;
	std::mutex lock;
	/**
	 * @brief initlize the winsock dll
	 *
	 * @return int greater-equel to zero if everything is ok
	 */
	int initWinsock();
	/**
	 * @brief initilize the adressInfo startuct that used by winsock
	 *
	 * @param [out result] a refrence to addrinfo struct that wil have the result
	 * @return int greater-equel to zero if everything is ok
	 */
	int initAddressInfo(addrinfo **result);
	/**
	 * @brief Create a Socket object that the server listen to
	 * @param [in addresInfo] source information to user
	 * @return new socket with the addrinfo
	 */
	SOCKET createSocket(addrinfo *addresInfo);

	void workerCleaner();

public:
	/**
	 * @brief Construct a new Server object
	 *
	 * @param [in Optinal port] number for the server to listen to.
	 */
	Server(std::string port = DEFUALT_PORT);
	/**
	 * @brief Initlize the server
	 *
	 * @return int greater-equel to zero if everything is ok
	 */
	int init();
	/**
	 * @brief get the new socket opend to the server, this function is blocking
	 *
	 * @param [out socket] newly created connection to the server
	 * @return int greater-equel to zero if everything is ok
	 */
	int nextSocket(SOCKET *socket);
	/**
	 * @brief Initiate worker
	 *
	 * @param soc for the worker work with
	 */
	void intiateWorker(SOCKET soc);
	~Server();
};
