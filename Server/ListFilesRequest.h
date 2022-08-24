#pragma once
#include "Common.h"
#include "RequestBase.h"
#include "Request.h"
#include "Response.h"
namespace Requests
{
	class ListFilesRequest : public RequestBase
	{
	private:
		// The request passed to handle
		Request *_req;
		// The socket to read and send data
		SOCKET _soc;

	public:
		/**
		 * @brief Get the File Request object
		 *
		 * @param request The request passed to handle
		 * @param soc The socket to read and send data
		 */
		ListFilesRequest(const Request &request, SOCKET soc);
		/**
		 * @brief Handles the request
		 *
		 * @return int non-negtaive result for OK
		 */
		int do_request();
	};

}
