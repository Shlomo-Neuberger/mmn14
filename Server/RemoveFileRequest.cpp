#include "RemoveFileRequest.h"

Requests::RemoveFileRequest::RemoveFileRequest(const Request &request, SOCKET soc)
{
	_req = new Request(request);
	_soc = soc;
}

int Requests::RemoveFileRequest::do_request()
{
	int iResult = 0;

	Responses::ResponseHeader header;
	Responses::ResponseBody body;
	std::string basepath(BASE_PATH); std::string filePath; std::ofstream outfile;
	// get the filename size
	char *buffer = new char[sizeof(_req->_header.fileLen)];
	ZeroMemory(buffer, sizeof(_req->_header.fileLen));
	size_t recived = recv(_soc, buffer, sizeof(_req->_header.fileLen), 0);
	iResult = _req->setFileLen(buffer, recived);
	delete[] buffer;
	if (iResult < 0) {
		iResult = NO_CONTENTS;
		header.status = STATUS_FAIL_SERVER_ERROR;
		header.version = VERSION;
		goto end;
	}
	// get the filename
	buffer = new char[_req->_header.fileLen + 1];
	ZeroMemory(buffer, _req->_header.fileLen + 1);
	recived = recv(_soc, buffer, _req->_header.fileLen, 0);
	iResult = _req->setFileName(buffer, recived);
	delete[] buffer;
	if (iResult < 0) {
		iResult = NO_CONTENTS;
		header.status = STATUS_FAIL_SERVER_ERROR;
		header.version = VERSION;
		goto end;
	}
	
	basepath += std::to_string(_req->_header.uid);
	filePath = basepath + "\\" + _req->_header.filename;
	outfile.open(filePath, std::ios::binary | std::ios::trunc | std::ios::in);
	if (!outfile.is_open())
	{
		iResult = NO_CONTENTS;
		header.status = STATUS_FAIL_FILE_NOT_FOUND;
		header.version = VERSION;
		goto end;
	}
	outfile.close();
	iResult = remove(filePath.c_str());
	if (iResult < 0) {
		iResult = NO_CONTENTS;
		header.status = STATUS_FAIL_SERVER_ERROR;
		header.version = VERSION;
		goto end;
	}
	iResult = NO_PAYLOAD;
	header.status = STATUS_SEUCCESS_FOUND_FILE;
	header.version = VERSION;
	header.fileLen = _req->getHeader().fileLen;
	header.filename = _req->getHeader().filename;
 end:
	Responses::sendResponse(_soc, &header, &body, iResult);
	return NO_PAYLOAD == iResult ? 0 : iResult; // A correct return value is NO_PAYLOAD so we rturn 0
}
