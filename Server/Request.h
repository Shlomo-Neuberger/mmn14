#pragma once
#include "Common.h"
#include "RequestBase.h"
namespace Requests {
	 
	typedef struct _RequestHeader {
		UINT32 uid =0;
		UINT8 version=0;
		UINT8 type = REQUEST_OP_UNKNOW_REQUEST;
		UINT16 fileLen = 0;
		char* filename=nullptr;
	}RequestHeader, *PRequestHeader;
	typedef struct _RequestBody {
		UINT32 fileSize = 0;
		byte* payload = nullptr;
	} RequestBody, *PRequestBody;
	class Request {
	private:
		int parseRequest(const char* buffer, UINT len, Request* request, byte flags = REQUEST_PARSER_PARSE_ALL);
		int parseRequestCommand(const char* buffer, UINT len, Request* request);;
		int parseRequestFileLen(const char* buffer, UINT len, Request* request);
		int parseRequestFileName(const char* buffer, UINT len, Request* request);
		int parseRequestPayloadSize(const char* buffer, UINT len, Request* request);
		int parseRequestPayload(const char* buffer, UINT len, Request* request);
	public:
		// Constracturs
		/*
		* Notice payload won't be deep copy by desing
		*/
		Request(const Request &old_obj);
		Request(char* buffer,size_t buffsize);
		Request();
		
		//Members
		RequestHeader _header;
		RequestBody _body;
		//Operators
		/**
			Notice  payload data isn't copy only refrnced
		*/
		Request& operator=(const Request& other);
		//Methods
		int setCommonHeader(const char* buffer, UINT len);
		int setFileLen(const char* buffer, UINT len);
		int setFileName(const char* buffer, UINT len);
		int setPayloadSize(const char* buffer, UINT len);
		const RequestHeader& getHeader();
		const RequestBody& getBody();
		virtual ~Request();
		
	};

}