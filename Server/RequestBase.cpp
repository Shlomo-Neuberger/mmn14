#include "RequestBase.h"
#include "Response.h"
Requests::RequestBase::~RequestBase() {}

int Requests::mkdirRecurse(std::string dir)
{
	std::istringstream input(dir);
	std::string temp;
	std::string created("");
	while (std::getline(input, temp, '\\'))
	{
		created += temp + "\\";
		if (_mkdir(created.c_str()) != -1)
		{
			std::cout << errno << "=?=" << ENOENT << std::endl;
			if (errno == ENOENT)
				return -1;
		}
	}
	return 0;
}
