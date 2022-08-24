#include "Common.h"
#include "RequestBase.h"
#include "Request.h"
#include "Response.h"
#include <fstream>
namespace Requests
{
	class GetFileRequest :public RequestBase
	{
	private:
		Request* _req;
		SOCKET _soc;

	public:
		GetFileRequest(const Request& request, SOCKET soc);
		int do_request(); // TODO: make return response
	};

}

