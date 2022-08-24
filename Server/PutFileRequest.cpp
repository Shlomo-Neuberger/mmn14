#include "PutFileRequest.h"

Requests::PutFileRequest::PutFileRequest(const Request &request, SOCKET soc)
{
	_req = new Request(request);
	_soc = soc;
}

int Requests::PutFileRequest::do_request()
{
	std::ofstream outfile;
	int iResult = 0;
	size_t buffer_size;
	std::string basepath;
	std::string filePath;
	size_t fileSize;
	size_t fileSizeMemberSize;
	Responses::ResponseHeader header;
	Responses::ResponseBody body;
	// get the filename size
	char *buffer = new char[sizeof(_req->_header.fileLen)];
	size_t recived = recv(_soc, buffer, sizeof(_req->_header.fileLen), 0);
	iResult = _req->setFileLen(buffer, recived);
	delete[] buffer;

	// get the filename
	buffer = new char[_req->_header.fileLen + 1];
	recived = recv(_soc, buffer, _req->_header.fileLen, 0);
	iResult = _req->setFileName(buffer, recived);
	delete[] buffer;
	if (iResult < 0) {
		iResult = NO_CONTENTS;
		header.status = STATUS_FAIL_SERVER_ERROR;
		header.version = VERSION;
		goto end;
	}

	// get the file size
	fileSizeMemberSize = sizeof(_req->_body.fileSize);
	buffer = new char[fileSizeMemberSize];
	recived = recv(_soc, buffer, fileSizeMemberSize, 0);
	iResult = _req->setPayloadSize(buffer, recived);
	delete[] buffer;
	if (iResult < 0) {
		iResult = NO_CONTENTS;
		header.status = STATUS_FAIL_SERVER_ERROR;
		header.version = VERSION;
		goto end;
	}

	buffer_size = min(DEFUALT_BUFFER_LEN, _req->_body.fileSize);
	fileSize = _req->_body.fileSize;
	recived = 0;
	basepath = BASE_PATH;
	basepath += std::to_string(_req->_header.uid);
	filePath = basepath + "\\" + _req->_header.filename;
	if (mkdirRecurse(basepath) != 0)
	{
		iResult = NO_CONTENTS;
		header.status = STATUS_FAIL_SERVER_ERROR;
		header.version = VERSION;
		goto end;
	}
	std::cout << "Saving " << filePath << std::endl;

	outfile.open(filePath, std::ios::binary | std::ios::app | std::ios::out);
	if (!outfile.is_open())
	{
		iResult = NO_CONTENTS;
		header.status = STATUS_FAIL_SERVER_ERROR;
		header.version = VERSION;
		goto end;
	}
	buffer = new char[buffer_size + 1];
	do
	{
		recived = recv(_soc, buffer, buffer_size, 0);
		fileSize -= recived;
		outfile.write(buffer, buffer_size);
	} while (fileSize > 0);
	outfile.close();
	delete[] buffer;
	iResult = FULL_RESPONSE;
	header.status = STATUS_SEUCCESS_FOUND_FILE;
	header.version = VERSION;
	header.fileLen = _req->getHeader().fileLen;
	header.filename = _req->getHeader().filename;
	body.fileSize = _req->getBody().fileSize;
	body.payload = _req->getBody().payload;
end:	
	Responses::sendResponse(_soc, &header, &body, iResult);
	return iResult;
}
