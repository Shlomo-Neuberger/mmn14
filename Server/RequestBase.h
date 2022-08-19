#pragma once
#define INALID_UID 0
#define PARSE_HEADER 1
#define PARSE_PAYLOAD_SIZE 2
#define PARSE_PAYLOAD_DATA 4
#define PARSE_ALL 7
#define PARSE_ALL 7
#define BUFFER_TO_SMALL -1
#define PUT_FILE 100
#define GET_FILE 200
#define REMOVE_FILE 201
#define DESCRIBE_FILES 202
#define UNKNOW_REQUEST 0
#include "Common.h"
#include "Request.h"
#include <sstream>
#include <direct.h>


namespace Requests {
	
	class RequestBase
	{
	public:
		virtual int do_request() = 0; // TODO: make return response
	};
	int mkdirRecurse(std::string dir);

	
}