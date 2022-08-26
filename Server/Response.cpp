#include "Response.h"

size_t Responses::sendResponse(const SOCKET soc, const PResponseHeader header, const PResponseBody body, int flag)
{
	size_t sent = 0;
	size_t bufSize = sizeof(header->version);
	char *base = (char *)&(header->version);
	char *buffer;
	std::vector<char> main_buffer;

	for (size_t i = 0; i < bufSize; i++)
	{
		main_buffer.push_back(*(base + i));
	}

	{
		bufSize = sizeof(header->status);

#ifdef BIG_ENDIAN
		UINT16 new_val = _byteswap_ushort(header->status);
#else
		UINT16 new_val = header->status;
#endif // BIG_ENDIAN
		base = (char *)&(new_val);
		for (size_t i = 0; i < bufSize; i++)
		{
			main_buffer.push_back(*(base + i));
		}
	}
	if (flag == RESPONSE_SENDER_NO_CONTENTS)
	{
		buffer = new char[main_buffer.size()];
		size_t i = 0;
		for (char b : main_buffer)
		{
			buffer[i++] = b;
		}
		sent = send(soc, buffer, (int)main_buffer.size(), NULL);
		delete[] buffer;
		return sent;
	}
	{
		bufSize = sizeof(header->fileLen);

#ifdef BIG_ENDIAN
		UINT16 new_val = _byteswap_ushort(header->fileLen);
#else
		UINT16 new_val = header->fileLen;
#endif // BIG_ENDIAN
		base = (char *)&(new_val);
		for (size_t i = 0; i < bufSize; i++)
		{
			main_buffer.push_back(*(base + i));
		}
	}

	bufSize = header->fileLen;
	base = header->filename;
	for (size_t i = 0; i < bufSize; i++)
	{
		main_buffer.push_back(*(base + i));
	}
	if (flag == RESPONSE_SENDER_NO_PAYLOAD)
	{
		buffer = new char[main_buffer.size()];
		size_t i = 0;
		for (char b : main_buffer)
		{
			buffer[i++] = b;
		}
		sent = send(soc, buffer, (int)main_buffer.size(), NULL);
		delete[] buffer;
		return sent;
	}
	{
		bufSize = sizeof(body->fileSize);

#ifdef BIG_ENDIAN
		UINT32 new_val = _byteswap_ulong(body->fileSize);
#else
		UINT32 new_val = body->fileSize;
#endif // BIG_ENDIAN
		base = (char *)&(new_val);
		for (size_t i = 0; i < bufSize; i++)
		{
			main_buffer.push_back(*(base + i));
		}
	}
	bufSize = body->fileSize;
	base = (char *)body->payload;
	for (size_t i = 0; i < bufSize; i++)
	{
		main_buffer.push_back(*(base + i));
	}
	buffer = new char[main_buffer.size()];
	size_t i = 0;
	for (char b : main_buffer)
	{
		buffer[i++] = b;
	}
	sent = send(soc, buffer, (int)main_buffer.size(), NULL);
	delete[] buffer;
	return sent;
}
