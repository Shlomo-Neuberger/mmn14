# Maman 14
We were task to create a backup server which stores a file for each user, and a client that can send the file to the server
# Server side

Compiled with:
* Windows 11
* Build tool v143
* Warning set to lvl-4
* Treat warning as error set to true

I used for server side the Winsock32 api provided by Windows Win32api.<br>
So the it runs only on windows.<br>
One of the biggesr problam I had is wthere recive the data in `Big endian` or `Small endian`, so ichoosed to handle both in the code and export the server config in the `Common.h`.<br>
All the configuration of the server are stored in `Common.h`.<br>
You can change the directory and other settings in the `Application Level`
```C
#pragma once
...
// Application level
#define VERSION 000
#define DEFUALT_PORT "8080"
#define DEFUALT_BUFFER_LEN 1024
#define BASE_PATH "C:\\backups\\"
// If BIG_ENDIAN defined the data shoudl arrive in BIG_ENDIAN format remove it if data is SmallEndian
#define BIG_ENDIAN
...
```

## Client
For clinet side I used the stander socket module that come in the python
