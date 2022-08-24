#pragma once

#include "Common.h"
#include "Request.h"
#include "Response.h"
#include <sstream>
#include <direct.h>
#include <sstream>
#include <direct.h>

namespace Requests
{

	class RequestBase
	{
	public:
		/**
		 * @brief Handles the request
		 *
		 * @return int non-negtaive result for OK
		 */
		virtual int do_request() = 0;
		virtual ~RequestBase();
	};
	/**
	 * @brief Creates a directory to recursavly
	 *
	 * @param dir the result wanted
	 * @return int non negative if ok
	 */
	int mkdirRecurse(std::string dir);

}