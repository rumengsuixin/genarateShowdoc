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
	// 初始化需要用到的堆空间
	jsondata = new T;
	tempjArr = new T;
}

template <typename T,typename C>
void analysisJson<T, C>::recursion(T* jsondata, std::vector<std::string>* veclist,C* Gd,callback cb) {

	// 遇数组则选第一个当作示例直接进入下一个递归
	if (jsondata->is_array()) {
		recursion(&((*jsondata)[0]), veclist,Gd, cb);
	}
	else {
	
		std::vector<std::string> temp;// 用以临时保存key容器状态( 局部变量 )
		T ison;// 用以临时保存 jsondata 子（json / my_json）对象( 局部变量 )

		(*cb)(Gd);// callback

		for (auto& i : jsondata->items()) {

			ison = i.value();


			if (jsondata->is_object()) {
				WriteIn(veclist, i.key(), ison,Gd, cb);
			}
			

			if (ison.is_object())// 对象
			{
				recursion(&ison, veclist,Gd, cb);
				if (!veclist->empty()) {
					veclist->pop_back();
				}
			}

			if (ison.is_array())// 数组
			{

				temp = *veclist;// 此处应该保留vector容器状态
				temp.pop_back();// 移除最新添加的一个成员
				recursion(&ison[0], veclist,Gd, cb);
				veclist->clear();// 递归数组完成之后清除容器
				*veclist = temp;// 恢复
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

		veclist->pop_back();// 移除自己
	}
	
}



template <typename T, typename C>
void analysisJson<T, C>::example(void) {

	this->recursion(this->getJson());

	//std::cout << this->getJson()->dump(4) << std::endl;

}

template <typename T, typename C>
void analysisJson<T, C>::recursion(T* jd) {
	
	// 如果是数组，则数组内只保留一个成员
	if (jd->is_array()) {
		// 创建临时 json 体，存储内容，

		tempjArr->push_back((*jd)[0]);
		*jd = *tempjArr;// 临时json体赋值
		// 使用完毕后对tempjArr clear
		tempjArr->clear();

		// 下面要进行递归 数组内唯一成员
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



