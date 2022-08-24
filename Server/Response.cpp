#include "Response.h"

int Responses::sendResponse(const SOCKET soc,const PResponseHeader header, const PResponseBody body,int flag)
{
	size_t sent = 0;
	size_t bufSize = sizeof(header->version);
	char* buffer = new char[bufSize];
	memcpy(buffer,&(header->version),bufSize);
	sent += send(soc, buffer, bufSize, 0);
	delete[] buffer;

	{
		bufSize = sizeof(header->status);
		buffer = new char[bufSize];
		UINT16 new_val = _byteswap_ushort(header->status);
		memcpy(buffer, &new_val, bufSize);
		sent += send(soc, buffer, bufSize, 0);
		delete[] buffer;
	}
	if (flag == RESPONSE_SENDER_NO_CONTENTS) {
		return sent;
	}
	{
		bufSize = sizeof(header->fileLen);
		buffer = new char[bufSize];
		UINT16 new_val = _byteswap_ushort(header->fileLen);
		memcpy(buffer, &(new_val), bufSize);
		sent += send(soc, buffer, bufSize, 0);
		delete[] buffer;
	}

	bufSize = header->fileLen;
	buffer = new char[bufSize];
	memcpy(buffer, header->filename, bufSize);
	sent += send(soc, buffer, bufSize, 0);
	delete[] buffer;
	if (flag == RESPONSE_SENDER_NO_PAYLOAD) {
		return sent;
	}
	{
		bufSize = sizeof(body->fileSize);
		buffer = new char[bufSize];
		UINT32 new_val = _byteswap_ulong(body->fileSize);
		memcpy(buffer, &(new_val), bufSize);
		sent += send(soc, buffer, bufSize, 0);
		delete[] buffer;
	}
	bufSize = body->fileSize;
	buffer = new char[bufSize];
	memcpy(buffer, body->payload, bufSize);
	sent += send(soc, buffer, bufSize, 0);
	delete[] buffer;

	return sent;
}
