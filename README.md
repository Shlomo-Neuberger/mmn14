# Maman 14
We were task to create a backup server which stores a file for each user, and a client that can send the file to the server
## Server side
<hr>
Compiled with:
<li>Windows 11</li>
<li>Build tool v143</li>
<li>Warning set to lvl-4</li>
<li>Treat warning as error set to true</li>
<hr>
I used for server side the Winsock32 api provided by Windows Win32api.<br>
So the it runs only on windows.<br>
One of the biggesr problam I had is wthere recive the data in `Big endian` or `Small endian`, so ichoosed to handle both in the code and export the server config in the `Common.h`.<br>
All the configuration of the server are stored in `Common.h`.<br>
You can change the directory and other settings in the `Application Level`
```C
#pragma once
#include <windows.h>
#pragma comment(lib, "User32.lib")
#include <string>
#include <algorithm>
#include <iostream>
#include <list>
#include <vector>
// Application level
#define VERSION 000
#define DEFUALT_PORT "8080"
#define DEFUALT_BUFFER_LEN 1024
#define BASE_PATH "C:\\backups\\"
// If BIG_ENDIAN defined the data shoudl arrive in BIG_ENDIAN format
#define BIG_ENDIAN

// PARSER FLAGS
#define REQUEST_PARSER_PARSE_HEADER 1
#define REQUEST_PARSER_PARSE_PAYLOAD_SIZE 2
#define REQUEST_PARSER_PARSE_PAYLOAD_DATA 4
#define REQUEST_PARSER_PARSE_ALL 7
// OP values
#define REQUEST_OP_PUT_FILE 100
#define REQUEST_OP_GET_FILE 200
#define REQUEST_OP_REMOVE_FILE 201
#define REQUEST_OP_LIST_FILES 202
#define REQUEST_OP_UNKNOW_REQUEST 0
#define CREATE_LIST_FILE_NAME_SIZE 32
// STATUS values
#define STATUS_SEUCCESS_FOUND_FILE 210
#define STATUS_SEUCCESS_LIST_CREATED 211
#define STATUS_FAIL_FILE_NOT_FOUND 1001
#define STATUS_FAIL_NO_FILES 1002
#define STATUS_FAIL_SERVER_ERROR 1003
// Response sendr flags
#define RESPONSE_SENDER_NO_CONTENTS -1
#define RESPONSE_SENDER_NO_PAYLOAD -2
#define RESPONSE_SENDER_FULL_RESPONSE 0
```

## Client
For clinet side I used the stander socket module that come in the python