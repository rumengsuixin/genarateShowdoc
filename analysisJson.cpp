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
	// ���ն��ڴ�
	if (jsondata) {
		delete jsondata;
	}
	if (tempjArr) {
		delete tempjArr;
	}
	//if (tempj) {
	//	delete tempj;
	//}
	
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
	//tempj = new T;
}

template<typename T, typename C>
void analysisJson<T, C>::analysisResponsedata(C* Gd)
{
	this->recursion(Gd->getExample(), Gd);//�ݹ������������������ص�����
}


template <typename T,typename C>
void analysisJson<T, C>::recursion(T* jsondata,C* Gd) {

	// ��������ѡ��һ������ʾ��ֱ�ӽ�����һ���ݹ�
	if (jsondata->is_array()) {
		recursion(&((*jsondata)[0]) ,Gd);
	}
	else {

	
		structCall(Gd);// ����һ�λص�����

		for (auto& i : jsondata->items()) {

	
			tempj = &(i.value());// ��ʱ json �ṹֵ�����仯
	

			if (jsondata->is_object()) {

				(*Gd->getVeclist()).push_back(i.key());// �����������һ����Ա����ԱΪ��ǰ key

				if (tempj->is_string() || tempj->is_number_integer() || tempj->is_null()) {// ����һ��Ҷ�ӣ�û���ӳ�Ա�ģ��ڵ�

					structCall(Gd, true);// ����һ�λص�����
					Gd->getVeclist()->pop_back();// ���������ӽ�������һ����Ա
				}

			}
			

			if (tempj->is_object())// ����
			{
				recursion(tempj, Gd);
				if (!Gd->getVeclist()->empty()) {
					Gd->getVeclist()->pop_back();
				}
			}

			if (tempj->is_array())// ����
			{
				//std::cout << "====================" << std::endl;
				//std::cout << (*tempj)[0].dump(4) << std::endl;
				//std::cout << Gd->getVeclist()->back() << std::endl;
				vectemp = *Gd->getVeclist();// �˴�Ӧ�ñ���vector����״̬
				vectemp.pop_back();// �Ƴ�������ӵ�һ����Ա
				recursion(&(*tempj)[0], Gd);// �ݹ������һ����Ա

				Gd->getVeclist()->clear();// �ݹ��������֮���������
				*Gd->getVeclist() = vectemp;// �ָ�
			}
		}
	}
}

template<typename T, typename C>
void analysisJson<T, C>::structCall(C* Gd, bool isLeaf)
{
	if (!Gd->getVeclist()->empty()) {
		(*Gd->getStructCallback())(Gd, isLeaf);// callback
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



