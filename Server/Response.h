#pragma once
#include "Common.h"

namespace Responses
{

	typedef struct _ResponseHeader
	{
		UINT8 version = VERSION;
		UINT16 status = STATUS_FAIL_SERVER_ERROR;
		UINT16 fileLen = 0;
		char *filename = nullptr;
	} ResponseHeader, *PResponseHeader;
	typedef struct _ResponseBody
	{
		UINT32 fileSize = 0;
		byte *payload = nullptr;
	} ResponseBody, *PResponseBody;
	/**
	 * @brief send a response
	 *
	 * @param soc distenation socket
	 * @param header source header
	 * @param body  source body
	 * @param flag what to send flag
	 * @return int non negative OK
	 */
	int sendResponse(const SOCKET soc, const PResponseHeader header, const PResponseBody body, int flag = RESPONSE_SENDER_NO_CONTENTS);

}