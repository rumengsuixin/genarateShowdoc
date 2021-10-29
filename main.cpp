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
	else if (arg->is_null()) {
		result = "string";
	}
	else {
		result = "unknown";
	}
	return result;
}



// 获取矢量数组对应示例最终json对象
my_json get_(my_json* example, vector<string>* v, bool isLeaf) {

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
				if (result[0].is_object()) {
					result = result[0][iter];
				}
				else {
					result = result[0];
				}
			}
		}
	}


	// 再进行一个 while 循环，直到准确定位正确节点（因为有可能多重数组嵌套）
	if (result.is_array() && isLeaf) {

		while (result.is_array()) {
			result = result[0];
		}
		result = result[*(v->end() - 1)];
	}

	return result;

}

string parameterLine(vector<string>::iterator& iter, vector<string>* v, GenerateDocument<my_json>* Gd, bool isLeaf) {

	my_json* example = Gd->getExample();
	my_json tempjson;// 使用 tempjson 来获取key表达的值
	string result;
	

	// 获取该条规则最终指向值
	tempjson = get_(example, v, isLeaf);

	result.append("|");
	for (; iter < v->end(); )
	{
		result.append(*iter);
		iter++;
		if (iter < v->end()) {
			result.append(".");
		}
	}
	// Data type
	result.append("|");
	result.append(entryType(&tempjson));
	result.append("|");


	// Data remarks


	if (Gd->getFieldConfig()->contains(*(iter - 1))) { // 能在json中找到索引
		result.append(Gd->getFieldConfig()->at(*(iter - 1)));
	}
	else {
		result.append("\t");
	}


	result.append("|");
	return result;
}
// 打印函数
static void stdlog(void* arg, bool isLeaf) {
	GenerateDocument<my_json>* gd = (GenerateDocument<my_json>*) arg;
	auto v = *gd->getVeclist();
	auto iter = v.begin();
	
	string str = parameterLine(iter, &v, gd, isLeaf);
	*gd->getOutfile() << str.c_str() << endl;// 写入文件
};




int main(){
	
	system("chcp 1258");

	GenerateDocument<my_json> gd(stdlog);

	gd.start();




	return 0;


}
