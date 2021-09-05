#include "analysisJson.h"


template <typename T,typename C>
analysisJson<T, C>::analysisJson(void)
{
	this->initialization();

}

template <typename T,typename C>
analysisJson<T, C>::analysisJson(json jsondata)
{
	this->initialization();
	*this->jsondata = jsondata;
}

template <typename T,typename C>
analysisJson<T, C>::analysisJson(my_json jsonobj)
{
	this->initialization();
	*this->jsondata = jsonobj;
}

template <typename T,typename C>
analysisJson<T, C>::analysisJson(std::string jsontext)
{
	this->initialization();
	this->modify(jsontext);
}

template <typename T, typename C>
analysisJson<T, C>::analysisJson(std::ifstream* stream)
{
	this->initialization();
	*this->jsondata = T::parse(*stream);
}

template <typename T,typename C>
analysisJson<T, C>::~analysisJson(void)
{
	if (jsondata) {
		delete jsondata;
	}
	if (tempjArr) {
		delete tempjArr;
	}
	
}
template <typename T, typename C>
void analysisJson<T, C>::modify(std::string jsontext) {

	*this->jsondata = T::parse(jsontext);

}

template <typename T,typename C>
T* analysisJson<T, C>::getJson(void)
{
	return jsondata;
}

template <typename T,typename C>
void analysisJson<T, C>::initialization(void) {
	// ��ʼ����Ҫ�õ��Ķѿռ�
	jsondata = new T;
	tempjArr = new T;
}

template <typename T,typename C>
void analysisJson<T, C>::recursion(T* jsondata, std::vector<std::string>* veclist,C* Gd,callback cb) {

	// ��������ѡ��һ������ʾ��ֱ�ӽ�����һ���ݹ�
	if (jsondata->is_array()) {
		recursion(&((*jsondata)[0]), veclist,Gd, cb);
	}
	else {
	
		std::vector<std::string> temp;// ������ʱ����key����״̬( �ֲ����� )
		T ison;// ������ʱ���� jsondata �ӣ�json / my_json������( �ֲ����� )

		(*cb)(Gd);// callback

		for (auto& i : jsondata->items()) {

			ison = i.value();


			if (jsondata->is_object()) {
				WriteIn(veclist, i.key(), ison,Gd, cb);
			}
			

			if (ison.is_object())// ����
			{
				recursion(&ison, veclist,Gd, cb);
				if (!veclist->empty()) {
					veclist->pop_back();
				}
			}

			if (ison.is_array())// ����
			{

				temp = *veclist;// �˴�Ӧ�ñ���vector����״̬
				temp.pop_back();// �Ƴ�������ӵ�һ����Ա
				recursion(&ison[0], veclist,Gd, cb);
				veclist->clear();// �ݹ��������֮���������
				*veclist = temp;// �ָ�
			}
		}
	
	}
}

template<typename T, typename C>
void analysisJson<T, C>::WriteIn(std::vector<std::string>* veclist, std::string key, T& ison,C* Gd, callback cb)
{

	(*veclist).push_back(key);

	if (ison.is_string() || ison.is_number_integer()) {

		(*cb)(Gd);// callback

		veclist->pop_back();// �Ƴ��Լ�
	}
	
}



template <typename T, typename C>
void analysisJson<T, C>::example(void) {

	this->recursion(this->getJson());

	//std::cout << this->getJson()->dump(4) << std::endl;

}

template <typename T, typename C>
void analysisJson<T, C>::recursion(T* jd) {
	
	// ��������飬��������ֻ����һ����Ա
	if (jd->is_array()) {
		// ������ʱ json �壬�洢���ݣ�

		tempjArr->push_back((*jd)[0]);
		*jd = *tempjArr;// ��ʱjson�帳ֵ
		// ʹ����Ϻ��tempjArr clear
		tempjArr->clear();

		// ����Ҫ���еݹ� ������Ψһ��Ա
		this->recursion(&(*jd)[0]);
		
	}
	else {
	
		for (auto& i : *jd) {
			if (i.is_object() || i.is_array()) {
				//std::cout << 111 << std::endl;
				this->recursion(&i);
			}
		}
	}
	

}



