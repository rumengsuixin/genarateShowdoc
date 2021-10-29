#include <string>
#include <iostream>
#include "Requester.h"
#include "GenerateShowdoc.cpp"
#include <array>


using namespace std;


// ��Ŀ���ͺ���
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



// ��ȡʸ�������Ӧʾ������json����
my_json get_(my_json* example, vector<string>* v, bool isLeaf) {

	my_json result;
	for (auto& iter : *v) {
		if (result.is_null()) {
			// result ��ʼ��
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


	// �ٽ���һ�� while ѭ����ֱ��׼ȷ��λ��ȷ�ڵ㣨��Ϊ�п��ܶ�������Ƕ�ף�
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
	my_json tempjson;// ʹ�� tempjson ����ȡkey����ֵ
	string result;
	

	// ��ȡ������������ָ��ֵ
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


	if (Gd->getFieldConfig()->contains(*(iter - 1))) { // ����json���ҵ�����
		result.append(Gd->getFieldConfig()->at(*(iter - 1)));
	}
	else {
		result.append("\t");
	}


	result.append("|");
	return result;
}
// ��ӡ����
static void stdlog(void* arg, bool isLeaf) {
	GenerateDocument<my_json>* gd = (GenerateDocument<my_json>*) arg;
	auto v = *gd->getVeclist();
	auto iter = v.begin();
	
	string str = parameterLine(iter, &v, gd, isLeaf);
	*gd->getOutfile() << str.c_str() << endl;// д���ļ�
};




int main(){
	
	system("chcp 1258");

	GenerateDocument<my_json> gd(stdlog);

	gd.start();




	return 0;


}
