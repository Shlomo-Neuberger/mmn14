#include "Request.h"

Request::Request::Request(const Request & old_obj)
{
	memcpy(&header, &(old_obj.header), sizeof(RequestHeader));
	header.filename = new char[old_obj.header.fileLen];
	memcpy(header.filename, old_obj.header.filename, old_obj.header.fileLen);
	memcpy(&body, &old_obj.body, sizeof(RequestBody));
}

Request::Request::Request(char * buffer, size_t buffsize)
{
	parseRequest(buffer, buffsize, this,PARSE_HEADER|PARSE_PAYLOAD_SIZE);
}

Request::Request::Request()
{
}

int Request::Request::setCommonHeader(const char * buffer, UINT len)
{
	return parseRequestCommand(buffer, len, this);
}

int Request::Request::setFileLen(const char * buffer, UINT len)
{
	return parseRequestFileLen(buffer, len, this);
}


int Request::Request::setFileName(const char * buffer, UINT len)
{
	return parseRequestFileName(buffer, len, this);
}

int Request::Request::setPayloadSize(const char* buffer, UINT len)
{
	return parseRequestPayloadSize(buffer, len, this);
}

Request::Request::~Request()
{
	if(this->header.filename != nullptr)
		delete this->header.filename;
	if (this->body.payload != nullptr)
		delete this->body.payload;
}



int Request::Request::parseRequest(const char * buffer, UINT len, Request * request, byte parseFlags)
{
	size_t commandSize = sizeof(request->header.uid) + sizeof(request->header.version) + sizeof(request->header.type);
	size_t nameLenSize = sizeof(request->header.fileLen);
	size_t payloadLensize = sizeof(request->body.fileSize);
	int iResult = 0;
	if (!request || request == nullptr) {
		return -1;
	}
	
	if (parseFlags == PARSE_ALL) {
		if (len >= commandSize && iResult == 0) iResult = parseRequestCommand(buffer,len,request);
		len -= commandSize;
		if (len >= nameLenSize && iResult == 0) iResult = parseRequestFileLen(buffer, len, request);
		len -= nameLenSize;
		if(len >= request->header.fileLen && iResult == 0) iResult = parseRequestFileName(buffer,len,request);
		len -= request->header.fileLen;
		if(len >= payloadLensize && iResult == 0) iResult = parseRequestPayloadSize(buffer + sizeof(RequestHeader), len - sizeof(RequestHeader), request);
		len -= payloadLensize;
		if(len >= request->body.fileSize && iResult == 0) iResult = parseRequestPayload(buffer + sizeof(RequestHeader) + sizeof(UINT), len - sizeof(RequestHeader), request);
		return  iResult;
	}
	if ((parseFlags & PARSE_HEADER) > 0) {
		parseRequestFileLen(buffer, len, request);
	}
	if ((parseFlags & PARSE_PAYLOAD_SIZE) > 0) {
		parseRequestPayloadSize(buffer, len, request);
	}
	if ((parseFlags & PARSE_PAYLOAD_DATA) > 0) {
		parseRequestPayload(buffer, len, request);
	}

	return 0;
}
int Request::Request::parseRequestFileLen(const char * buffer, UINT len, Request * request)
{
	size_t payloadSize = sizeof(request->header.fileLen);
	if (len < payloadSize) {
		return -1;
	}
	memcpy(&(request->header.fileLen), buffer, payloadSize);
	request->header.fileLen = _byteswap_ushort(request->header.fileLen);
	return 0;
}
int Request::Request::parseRequestCommand(const char * buffer, UINT len, Request * request)
{
	size_t payloadSize = sizeof(request->header.uid)+ sizeof(request->header.version)+ sizeof(request->header.type);
	if (len < payloadSize) {
		return -1;
	}
	memcpy(&request->header, buffer, payloadSize);
	request->header.uid = _byteswap_ulong(request->header.uid);
	return 0;
}
int Request::Request::parseRequestFileName(const char * buffer, UINT len, Request * request)
{
	request->header.filename = new char[min(UINT16_MAX - 1, request->header.fileLen + 1)];
	memcpy(request->header.filename, buffer, request->header.fileLen);
	request->header.filename[request->header.fileLen] = 0;
	return 0;
}
int Request::Request::parseRequestPayloadSize(const char * buffer, UINT len, Request * request)
{
	memcpy(&(request->body.fileSize), buffer, sizeof(request->body.fileSize));
	request->body.fileSize = _byteswap_ulong(request->body.fileSize);
	return 0;
}
int Request::Request::parseRequestPayload(const char * buffer, UINT len, Request * request)
{
	return 0;
}