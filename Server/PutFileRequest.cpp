#include "PutFileRequest.h"

Requests::PutFileRequest::PutFileRequest(const Request &request,SOCKET soc)
{
	_req = new Request(request);
	_soc = soc;
}

Requests::Request* Requests::PutFileRequest::getRequest()
{
	return _req;
}

int Requests::PutFileRequest::do_request()
{
	int iResult = 0;
	// get the filename size
	char* buffer = new char[sizeof(_req->header.fileLen)];
	size_t recived = recv(_soc, buffer, sizeof(_req->header.fileLen),0);
	iResult = _req->setFileLen(buffer, recived);
	delete[] buffer;

	// get the filename
	buffer = new char[_req->header.fileLen+1];
	recived = recv(_soc, buffer, _req->header.fileLen, 0);
	iResult = _req->setFileName(buffer, recived);
	delete[] buffer;
	
	//get the file size
	size_t fileSizeMemberSize = sizeof(_req->body.fileSize);
	buffer = new char[fileSizeMemberSize];
	recived = recv(_soc, buffer, fileSizeMemberSize,0);
	_req->setPayloadSize(buffer, recived);
	delete[] buffer;

	size_t buffer_size = min(DEFUALT_BUFFER_LEN, _req->body.fileSize);
	buffer = new char[buffer_size+1];
	size_t fileSize = _req->body.fileSize;
	recived = 0;
	std::string basepath(BASE_PATH);
	basepath += std::to_string(_req->header.uid);
	std::string filePath = basepath + "\\" + _req->header.filename;
	if (mkdirRecurse(basepath) != 0) {
		return -1;
	}
	std::cout << "Saving " << filePath << std::endl;
	std::ofstream outfile(filePath, std::ios::binary | std::ios::app | std::ios::out);
	if(!outfile.is_open()) return -1;
	
	do {
		recived = recv(_soc, buffer, buffer_size, 0);
		fileSize -= recived;
		outfile.write(buffer, buffer_size);
	} while (fileSize > 0);
	outfile.close();
	delete[] buffer;
	return 0;
}

