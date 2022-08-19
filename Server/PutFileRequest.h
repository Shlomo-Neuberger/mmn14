#pragma once
#include "Common.h"
#include "RequestBase.h"
#include "Request.h"
#include <fstream>
#include <sstream>
#include <direct.h>
namespace Request {
	class PutFileRequest :
		public RequestBase
	{
	private:
		Request* _req;
		SOCKET _soc;
		
	public:
		PutFileRequest(const Request &request,SOCKET soc);
		UINT32 getUID() noexcept;
		UINT8 getType() noexcept;
		Request* getRequest();
		int do_request(); // TODO: make return response
	};
	int mkdirRecurse(std::string dir);
}
