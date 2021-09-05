#include "Requester.h"


requests::Response::Response(void) {

	//实现 Response 构造方法
}

Requester::Requester(void)
{
	method = "GET";
	url = "";
	ResponseBody = new Response;
}

Requester::~Requester(void)
{
	delete ResponseBody;
}

void Requester::get(string url)
{
	this->url = url;
	*ResponseBody = requests::Get(this->url);
}

requests::Response* Requester::Getbody(void)
{
	return ResponseBody;
}
