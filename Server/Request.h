#pragma once
#include "Common.h"
#include "RequestBase.h"
namespace Requests
{
	/**
	 * @brief Basic struct for the parts of the request Header
	 *
	 */
	typedef struct _RequestHeader
	{
		UINT32 uid = 0;
		UINT8 version = 0;
		UINT8 type = REQUEST_OP_UNKNOW_REQUEST;
		UINT16 fileLen = 0;
		char *filename = nullptr;
	} RequestHeader, *PRequestHeader;
	/**
	 * @brief Basic struct for the parts of the request Body
	 *
	 */
	typedef struct _RequestBody
	{
		UINT32 fileSize = 0;
		byte *payload = nullptr;
	} RequestBody, *PRequestBody;
	class Request
	{
	private:
		int parseRequest(const char *buffer, UINT len, Request *request, byte flags = REQUEST_PARSER_PARSE_ALL);
		int parseRequestCommand(const char *buffer, UINT len, Request *request);
		int parseRequestFileLen(const char *buffer, UINT len, Request *request);
		int parseRequestFileName(const char *buffer, UINT len, Request *request);
		int parseRequestPayloadSize(const char *buffer, UINT len, Request *request);
		int parseRequestPayload(const char *buffer, UINT len, Request *request);

	public:
		// Members
		// header of the request
		RequestHeader _header;
		// body of the request
		RequestBody _body;
		UINT _postion = 0;
		// Constracturs
		/*
		 * Notice payload won't be deep copy by desing
		 */
		/**
		 * @brief Construct a new Request object
		 *
		 * @param old_obj to copy from
		 */
		Request(const Request &old_obj);
		/**
		 * @brief Construct a new Request object from buffer
		 *
		 * @param buffer the source
		 * @param buffsize size of buffer
		 */
		Request(char *buffer, size_t buffsize);
		/**
		 * @brief Construct a new Request object
		 *
		 */
		Request();

		// Operators
		/**
			Notice  payload data isn't copy only refrnced
		*/
		Request &operator=(const Request &other);
		// Methods
		/**
		 * @brief Set the Common Header object
		 *
		 * @param buffer source
		 * @param len buffer size
		 * @return int non-negtive for OK
		 */
		int setCommonHeader(const char *buffer, UINT len);
		/**
		 * @brief Set the File Len object
		 * @param buffer source
		 * @param len buffer size
		 * @return int non-negtive for OK
		 */
		int setFileLen(const char *buffer, UINT len);
		/**
		 * @brief Set the File Name object
		 *
		 * @param buffer source
		 * @param len buffer size
		 * @return int non-negtive for OK
		 */
		int setFileName(const char *buffer, UINT len);
		/**
		 * @brief Set the Payload Size object
		 *
		 * @param buffer source
		 * @param len buffer size
		 * @return int non-negtive for OK
		 */
		int setPayloadSize(const char *buffer, UINT len);

		/**
		 * @brief Get the Header object
		 *
		 * @return const RequestHeader&
		 */
		const RequestHeader &getHeader();
		/**
		 * @brief Get the Body object
		 *
		 * @return const RequestBody&
		 */
		const RequestBody &getBody();
		/**
		 * @brief Destroy the Request object
		 *
		 */
		virtual ~Request();
	};

}