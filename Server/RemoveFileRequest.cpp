#include "RemoveFileRequest.h"

Requests::RemoveFileRequest::RemoveFileRequest(const Request & request, SOCKET soc)
{
	_req = new Request(request);
	_soc = soc;

}

int Requests::RemoveFileRequest::do_request()
{
	int iResult = 0;
	// get the filename size
	char* buffer = new char[sizeof(_req->header.fileLen)];
	ZeroMemory(buffer, sizeof(_req->header.fileLen));
	size_t recived = recv(_soc, buffer, sizeof(_req->header.fileLen), 0);
	iResult = _req->setFileLen(buffer, recived);
	delete[] buffer;

	// get the filename
	buffer = new char[_req->header.fileLen + 1];
	ZeroMemory(buffer, _req->header.fileLen + 1);
	recived = recv(_soc, buffer, _req->header.fileLen, 0);
	iResult = _req->setFileName(buffer, recived);
	delete[] buffer;

	std::string basepath(BASE_PATH);
	basepath += std::to_string(_req->header.uid);
	std::string filePath = basepath + "\\" + _req->header.filename;
	std::ofstream outfile(filePath, std::ios::binary | std::ios::trunc | std::ios::in);
	if (!outfile.is_open()) return -1;
	outfile.close();
	return remove(filePath.c_str());
}
