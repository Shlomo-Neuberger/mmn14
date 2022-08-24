#include "ListFilesRequest.h"

Requests::ListFilesRequest::ListFilesRequest(const Request& request, SOCKET soc)
{
	_req = new Request(request);
	_soc = soc;
}



int Requests::ListFilesRequest::do_request()
{
	WIN32_FIND_DATAA data;
	HANDLE hFind;
	size_t skips;
	std::string filename("");
	std::string lsPath(BASE_PATH);
	Responses::ResponseHeader header;
	Responses::ResponseBody body;
	std::vector<std::string> fileData;
	int iResult = !0;
	static const char alphanum[] =
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	body.fileSize = 0;
	if (_req->getHeader().type != LIST_FILES) {
		iResult = NO_CONTENTS;
		header.status = STATUS_FAIL_SERVER_ERROR;
		header.version = VERSION;
		goto end;
	}
	lsPath += std::to_string(_req->getHeader().uid)+"\\*";
	if (strlen(lsPath.c_str()) > MAX_PATH) {
		iResult = NO_CONTENTS;
		header.status = STATUS_FAIL_SERVER_ERROR;
		header.version = VERSION;
		goto end;
	}
	hFind = FindFirstFileA(lsPath.c_str(), &data);
	srand((UINT)time(0));
	for (int i = 0; i < FILE_NAME_SIZE; i++) {
		filename += alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			std::string _tmp = std::string(data.cFileName);
			if (_tmp == "." || _tmp == "..") continue;
			UINT64 nextSize = ((UINT64)body.fileSize) + _tmp.length();
			if (nextSize > UINT32_MAX)
			{ //Protect file size overflow
				iResult = NO_CONTENTS;
				header.status = STATUS_FAIL_SERVER_ERROR;
				header.version = VERSION;
				goto end;
			}
			body.fileSize += _tmp.length();
			fileData.push_back(_tmp);
		} while (FindNextFileA(hFind, &data));
		FindClose(hFind);
	}
	
	if (fileData.size() == 0 || hFind != INVALID_HANDLE_VALUE) {
		iResult = NO_CONTENTS;
		header.status = STATUS_FAIL_NO_FILES;
		header.version = VERSION;
		goto end;
	}

	header.version = VERSION;
	header.status = STATUS_SEUCCESS_LIST_CREATED;
	header.fileLen = FILE_NAME_SIZE;
	header.filename = new char[header.fileLen];
	memcpy(header.filename, filename.c_str(), FILE_NAME_SIZE);
	body.fileSize += fileData.size();
	body.payload = new byte[body.fileSize];
	skips = 0;
	for (auto tmpFilename : fileData)
	{
		memcpy(body.payload + skips, tmpFilename.c_str(), tmpFilename.length() + 1);
		skips += tmpFilename.length();
		body.payload[skips] = (byte)'\n';
	}
	iResult = FULL_RESPONSE;
end:
	Responses::sendResponse(_soc, &header, &body, iResult);
	if(body.payload!= nullptr) delete[] body.payload;
	if (header.filename != nullptr) delete[] header.filename;
	return iResult;
}
