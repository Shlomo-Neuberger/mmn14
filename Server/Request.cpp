#include "Request.h"

Requests::Request::Request(const Request & old_obj)
{
	memcpy(&_header, &(old_obj._header), sizeof(RequestHeader));
	_header.filename = new char[old_obj._header.fileLen];
	memcpy(_header.filename, old_obj._header.filename, old_obj._header.fileLen);
	memcpy(&_body, &old_obj._body, sizeof(RequestBody));
}

Requests::Request::Request(char * buffer, size_t buffsize)
{
	parseRequest(buffer, buffsize, this,PARSE_HEADER|PARSE_PAYLOAD_SIZE);
}

Requests::Request::Request()
{
}

Requests::Request& Requests::Request::operator=(const Request& other)
{
	if (this == &other) {
		return *this;
	}
	memcpy(&(this->_header), &(other._header), sizeof(RequestHeader));
	this->_header.filename = new char[other._header.fileLen];
	memcpy(this->_header.filename, other._header.filename, other._header.fileLen);
	memcpy(&(this->_body), &other._body, sizeof(RequestBody));
	return *this;
}

int Requests::Request::setCommonHeader(const char * buffer, UINT len)
{
	return parseRequestCommand(buffer, len, this);
}

int Requests::Request::setFileLen(const char * buffer, UINT len)
{
	return parseRequestFileLen(buffer, len, this);
}


int Requests::Request::setFileName(const char * buffer, UINT len)
{
	return parseRequestFileName(buffer, len, this);
}

int Requests::Request::setPayloadSize(const char* buffer, UINT len)
{
	return parseRequestPayloadSize(buffer, len, this);
}

const Requests::RequestHeader& Requests::Request::getHeader()
{
	// TODO: insert return statement here
	return this->_header;
}

const Requests::RequestBody& Requests::Request::getBody()
{
	// TODO: insert return statement here
	return this->_body;
}

Requests::Request::~Request()
{
	if(this->_header.filename != nullptr)
		delete this->_header.filename;
	if (this->_body.payload != nullptr)
		delete this->_body.payload;
}



int Requests::Request::parseRequest(const char * buffer, UINT len, Request * request, byte parseFlags)
{
	size_t commandSize = sizeof(request->_header.uid) + sizeof(request->_header.version) + sizeof(request->_header.type);
	size_t nameLenSize = sizeof(request->_header.fileLen);
	size_t payloadLensize = sizeof(request->_body.fileSize);
	int iResult = 0;
	if (!request || request == nullptr) {
		return -1;
	}
	
	if (parseFlags == PARSE_ALL) {
		if (len >= commandSize && iResult == 0) iResult = parseRequestCommand(buffer,len,request);
		len -= commandSize;
		if (len >= nameLenSize && iResult == 0) iResult = parseRequestFileLen(buffer, len, request);
		len -= nameLenSize;
		if(len >= request->_header.fileLen && iResult == 0) iResult = parseRequestFileName(buffer,len,request);
		len -= request->_header.fileLen;
		if(len >= payloadLensize && iResult == 0) iResult = parseRequestPayloadSize(buffer + sizeof(RequestHeader), len - sizeof(RequestHeader), request);
		len -= payloadLensize;
		if(len >= request->_body.fileSize && iResult == 0) iResult = parseRequestPayload(buffer + sizeof(RequestHeader) + sizeof(UINT), len - sizeof(RequestHeader), request);
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
int Requests::Request::parseRequestFileLen(const char * buffer, UINT len, Request * request)
{
	size_t payloadSize = sizeof(request->_header.fileLen);
	if (len < payloadSize) {
		return -1;
	}
	memcpy(&(request->_header.fileLen), buffer, payloadSize);
	request->_header.fileLen = _byteswap_ushort(request->_header.fileLen);
	return 0;
}
int Requests::Request::parseRequestCommand(const char * buffer, UINT len, Request * request)
{
	size_t payloadSize = sizeof(request->_header.uid)+ sizeof(request->_header.version)+ sizeof(request->_header.type);
	if (len < payloadSize) {
		return -1;
	}
	memcpy(&request->_header, buffer, payloadSize);
	request->_header.uid = _byteswap_ulong(request->_header.uid);
	return 0;
}
int Requests::Request::parseRequestFileName(const char * buffer, UINT len, Request * request)
{
	size_t fileNameSize = min(UINT16_MAX - 1, request->_header.fileLen + 1);
	if (len < fileNameSize-1) {
		return -1;
	}
	request->_header.filename = new char[fileNameSize];
	memcpy(request->_header.filename, buffer, fileNameSize);
	request->_header.filename[fileNameSize] = 0;
	return 0;
}
int Requests::Request::parseRequestPayloadSize(const char * buffer, UINT len, Request * request)
{
	size_t fileSizeDataSize = sizeof(request->_body.fileSize);
	if (len < fileSizeDataSize) {
		return -1;
	}
	memcpy(&(request->_body.fileSize), buffer, fileSizeDataSize);
	request->_body.fileSize = _byteswap_ulong(request->_body.fileSize);
	return 0;
}
int Requests::Request::parseRequestPayload(const char * buffer, UINT len, Request * request)
{
	UNREFERENCED_PARAMETER(buffer);
	UNREFERENCED_PARAMETER(len);
	UNREFERENCED_PARAMETER(request);
	return 0;
}