#include "GenerateShowdoc.h"



const std::string templatePath = "E:\\showdoc\\showdoc.template";
const std::string configPath = "E:\\showdoc\\config.json";
const std::string outPath = "E:\\showdoc\\out.txt";
const std::string sourceUrl = "http://ynysxy01.com/lotynnf/loto";



template<typename T>
GenerateDocument<T>::GenerateDocument(void) {
	
	

	//// 打开文件流
	buffer.open(configPath);// 只读方式打开配置文件
	
	ajson = new analysisJson<T, GenerateDocument<T>>(&buffer); // json 解析器
	tj = new analysisJson<T, GenerateDocument<T>>; // 响应体 json 解析器
	requester = new Requester;// 请求器
	config = new T;

	buffer.close();// 关闭文件流
	buffer.clear();// 清除文件流状态
	buffer.open(templatePath);// 只读方式打开模板文件

	outfile.open(outPath);// 只读方式打开输出文件
	//正则对象
	pattern = new std::regex("\\{([a-zA-Z]{1,})\\}");

}
template<typename T>
GenerateDocument<T>::~GenerateDocument(void) {
	// 关闭文件流
	if (buffer.is_open()) {
		buffer.close();
	}
	if (outfile.is_open()) {
		outfile.close();
	}
	// 释放json解析器堆内存
	if (ajson) {
		delete ajson;
	}
	if (tj) {
		delete tj;
	}
	if (requester) {
		delete requester;
	}
	if (config) {
		delete config;
	}
	if (pattern) {
		delete pattern;
	}
}




// 开始运行函数
template<typename T>
void GenerateDocument<T>::start(callback cb) {


	// read config into GenerateDocument of instance
	*config = (*ajson->getJson())["list"];


	// 发送请求
	requester->get((*config)["sourceUrl"]["value"]);



	// 解析返回响应体参数
	tj->modify(requester->Getbody()->GetText());



	// 初始化 config 
	if (requester->Getbody()) {

		// 指定初始化内容

		// 构造返回参数样例
		tj->example();


		for (auto& item : config->items())
		{
			switch ((int)item.value()["type"]) {
			case 2:// 根据请求者获取请求数据
				//std::cout << "请求数据: " << std::endl;
				break;
			case 3:
				// 初始化返回json数据
				
				break;
			case 4:// 初始化 示例 数据

				(item.value())["value"] = *this->getExample();
				break;
			default:
				break;
			}
		}
	}


	// config 结合 请求者数据初始化完毕


	// 逐行读取并进行解析和写入
	while (getline(buffer, flush)) {
		parseAndWrite(cb);
	}
}



// 解析 config 并写入 outfile 文件流
template<typename T>
void GenerateDocument<T>::parseAndWrite(callback cb) {
	// 检测行内是否有替换的参数
	if (isContain(&flush)) {
		int type;
		//std::string temp;
		std::smatch result;
		std::string::const_iterator iter_begin = flush.cbegin();
		std::string::const_iterator iter_end = flush.cend();

		// 循环正则匹配 一直到没有结果
		while (regex_search(iter_begin, iter_end, result, *pattern)) {
			//temp.append(result.prefix());
			outfile << result.prefix(); // 写入本条匹配结果的前一段内容
			type = (int)(*config)[result[1]]["type"];



			// 根据类型写入期望值
			switch (type) {
			case 2:// 请求参数
				outfile << (*config)[result[1]]["value"].dump(4);
				break;
			case 3:// 返回参数
				// 递归解析json 到返回参数列表中
				tj->recursion(this->getExample(), this->getVeclist(), this, cb);// 把写入权交给 stdlog 函数
				break;
			case 4:// 示例
				outfile << (*config)[result[1]]["value"].dump(4);
				break;
			default:
				//temp.append((std::string)(*config)[result[1]]["value"]);
				outfile << (std::string)(*config)[result[1]]["value"];
				break;
			}
			iter_begin = result[0].second;// 获取本次匹配的字符串在源字符串中下一个迭代器的位置

		}
		//temp.append(result.suffix());// 最后一段
		outfile << result.suffix(); // 写入本条匹配结果后一段的内容

		//outfile << temp << std::endl;// 写入一行
		outfile << std::endl;// 写入换行符
	}
	else {

		outfile << flush << std::endl;

	}


}


template<typename T>
T* GenerateDocument<T>::getConfig(void)
{
	return config;
}

template<typename T>
T* GenerateDocument<T>::getExample(void)
{
	return tj->getJson();
}



template<typename T>
ofstream* GenerateDocument<T>::getOutfile(void) {

	return &outfile;

}
template<typename T>
std::vector<string>* GenerateDocument<T>::getVeclist(void) {

	return &veclist;

}



// 检索是否含有
template<typename T>
bool GenerateDocument<T>::isContain(std::string* str) {

	bool ret = FALSE;
	std::regex pattern("\\{[a-zA-Z]+\\}");

	ret = regex_search(*str, pattern);

	return ret;
}
