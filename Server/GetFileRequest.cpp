#include "GetFileRequest.h"

Requests::GetFileRequest::GetFileRequest(const Request& request, SOCKET soc)
{
	_req = new Request(request);
	_soc = soc;
}



int Requests::GetFileRequest::do_request()
{
	std::string filename("");
	std::string lsPath(BASE_PATH);
	Responses::ResponseHeader header;
	Responses::ResponseBody body;
	std::filebuf* pbuf = nullptr;
	UINT64 fileSize = 0;
	size_t iResult = 0;
	int retVal = -1;
	char* fileData = nullptr;
	char* buffer = new char[sizeof(_req->_header.fileLen)];
	size_t recived = recv(_soc, buffer, sizeof(_req->_header.fileLen), 0);
	iResult = _req->setFileLen(buffer, recived);
	delete[] buffer;

	// get the filename
	buffer = new char[_req->_header.fileLen + 1];
	recived = recv(_soc, buffer, _req->_header.fileLen, 0);
	iResult = _req->setFileName(buffer, recived);
	delete[] buffer;

	body.fileSize = 0;
	lsPath += std::to_string(_req->getHeader().uid)+"\\"+ _req->getHeader().filename;
	if (strlen(lsPath.c_str()) > MAX_PATH) {
		return -1;
	}

	std::ifstream ifile(lsPath,std::ios::binary);
	if (!ifile.is_open())
	{
		retVal = RESPONSE_SENDER_NO_PAYLOAD;
		header.status = STATUS_FAIL_FILE_NOT_FOUND;
		header.version = VERSION;
		header.fileLen = _req->getHeader().fileLen;
		header.filename = _req->getHeader().filename;
		goto end;
	}

	// get pointer to associated buffer object
	pbuf = ifile.rdbuf();

	// get file size using buffer's members
	fileSize = pbuf->pubseekoff(0, ifile.end, ifile.in);
	pbuf->pubseekpos(0, ifile.in);
	if (fileSize > UINT32_MAX) {
		retVal = RESPONSE_SENDER_NO_CONTENTS;
		header.status = STATUS_FAIL_SERVER_ERROR;
		header.version = VERSION;
		goto end;
	}
	// allocate memory to contain file data
	fileData = new char[(UINT32)fileSize];

	// get file data
	pbuf->sgetn(fileData, fileSize);

	ifile.close();
	retVal = RESPONSE_SENDER_FULL_RESPONSE;
	header.version = VERSION;
	header.status = STATUS_SEUCCESS_LIST_CREATED;
	header.fileLen = _req->getHeader().fileLen;
	header.filename = new char[header.fileLen];
	memcpy(header.filename, _req->getHeader().filename, header.fileLen);
	body.fileSize = (UINT32)fileSize;
	body.payload = (byte*)fileData;
end:
	Responses::sendResponse(_soc, &header, &body, retVal);
	if(nullptr != body.payload)delete[] body.payload;
	if (nullptr != header.filename) delete[] header.filename;
	return retVal;

}
