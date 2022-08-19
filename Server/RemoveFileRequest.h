#pragma once
#include "Common.h"
#include "RequestBase.h"
#include <fstream>
namespace Requests {
	class RemoveFileRequest :
		public RequestBase
	{
	private:
		Request* _req;
		SOCKET _soc;
	public:
	public:
		RemoveFileRequest(const Request &request, SOCKET soc);
		int do_request(); // TODO: make return response

	};
}

