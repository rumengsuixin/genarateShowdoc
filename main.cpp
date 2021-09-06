#include <string>
#include <iostream>
#include "Requester.h"
#include "GenerateShowdoc.cpp"
#include <array>


using namespace std;


// 条目类型函数
string entryType(my_json* arg) {
	string result;
	if (arg->is_array()) {
		result = "array";
	}
	else if (arg->is_object()) {
	
		result = "object";
	}
	else if (arg->is_boolean())
	{
		result = "boolean";
	}
	else if (arg->is_string())
	{
		result = "string";
	}
	else if (arg->is_number_integer())
	{
		result = "int";
	}
	else {
		result = "unknown";
	}
	return result;
}


my_json get_(my_json* example, vector<string>* v) {

	my_json result;

	for (auto& iter : *v) {

		if (result.is_null()) {
			// result 初始化
			if (example->is_object()) {
				result = (*example)[*v->begin()];
			}
			// else
		}
		else {
			if (result.is_object()) {
				result = result[iter];
			}
			else if (result.is_array()) {
				result = result[0][iter];
			}
		}
	}

	return result;

}

string parameterLine(vector<string>::iterator& iter, vector<string>* v, GenerateDocument<my_json>* Gd) {

	my_json* example = Gd->getExample();
	my_json tempjson;// 使用 tempjson 来获取key表达的值
	string result;
	


	// 获取该条规则最终指向值
	tempjson = get_(example, v);
	
	result.append("|");
	for (; iter < v->end(); )
	{
		result.append(*iter);
		iter++;
		if (iter < v->end()) {
			result.append(".");
		}
	}
	result.append("|");


	result.append(entryType(&tempjson));
	result.append("|");
	result.append("\t");
	result.append("|");
	return result;
}
// 打印函数
static void stdlog(void* arg) {
	GenerateDocument<my_json>* gd = (GenerateDocument<my_json>*) arg;
	auto v = *gd->getVeclist();
	auto iter = v.begin();



	string str = parameterLine(iter,&v,gd);
	*gd->getOutfile() << str.c_str() << endl;// 写入文件
};



int main(){





	GenerateDocument<my_json> gd;
	gd.start(stdlog);

	return 0;


}
