#pragma once
#include "requests.h"



using namespace requests;

class Requester {
public:
	Requester(void);
	~Requester(void);

	void get(std::string url);
	Response* Getbody(void);

private:
	std::string method;
	std::string url;
	Response* ResponseBody;
	
};