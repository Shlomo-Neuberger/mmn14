#pragma once
#include "Common.h"
#include "RequestBase.h"
#include "Request.h"
#include <fstream>
namespace Requests
{
	class PutFileRequest : public RequestBase
	{
	private:
		Request *_req;
		SOCKET _soc;

	public:
		PutFileRequest(const Request &request, SOCKET soc);
		int do_request(); // TODO: make return response
	};

}
