#pragma once
#include "Common.h"
#include "RequestBase.h"
#include "Request.h"
#include "Response.h"
namespace Requests
{
    class ListFilesRequest :public RequestBase
	{
	private:
		Request* _req;
		SOCKET _soc;

	public:
		ListFilesRequest(const Request& request, SOCKET soc);
		int do_request(); // TODO: make return response
	};

}

