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
		virtual int do_request() = 0; // TODO: make return response
		virtual ~RequestBase();
	};

	int mkdirRecurse(std::string dir);

}