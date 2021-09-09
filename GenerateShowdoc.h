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
	analysisJson<T, GenerateDocument>* ajson;// config json ������
	analysisJson<T, GenerateDocument>* tj;// return data json ������
	Requester* requester;// ������
	T* config;// ����

	std::regex* pattern;// ��̬�������
	std::string flush;
	std::ifstream buffer;
	std::ofstream outfile;

	std::vector<string> veclist;// �ַ�������������ִ��ʱ������json�������ݹ���ö��仯��



	callback structCallback; // json�ṹ�ص�����ָ��



	void parseAndWrite(callback cb);
	bool isContain(std::string* str);



public:
	GenerateDocument(callback cb);
	~GenerateDocument(void);
	void start();// ��ʼ���ļ���������ȡ����
	T* getConfig(void);
	T* getExample(void);


	ofstream* getOutfile(void);
	std::vector<string>* getVeclist(void);
	callback getStructCallback(void);

};




