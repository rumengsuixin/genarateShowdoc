#pragma once
#include <regex>
#include <fstream>
#include "Requester.h"
#include "analysisJson.cpp"

//#define TRUE true
//#define FALSE false

const char sleft = '{';
const char sright = '}';




template<typename T>
class GenerateDocument {
private:
	//T* config;
	analysisJson<T, GenerateDocument>* ajson;// config json 解析器
	analysisJson<T, GenerateDocument>* tj;// return data json 解析器
	Requester* requester;// 请求器
	T* config;// 配置

	std::regex* pattern;// 静态正则对象
	std::string flush;
	std::ifstream buffer;
	std::ofstream outfile;

	std::vector<string> veclist;// 字符串容器（程序执行时，由于json解析器递归调用而变化）



	callback structCallback; // json结构回调函数指针



	void parseAndWrite(callback cb);
	bool isContain(std::string* str);



public:
	GenerateDocument(callback cb);
	~GenerateDocument(void);
	void start();// 开始从文件流逐条读取数据
	T* getConfig(void);
	T* getExample(void);


	ofstream* getOutfile(void);
	std::vector<string>* getVeclist(void);
	callback getStructCallback(void);

};




