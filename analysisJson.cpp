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
	// 回收堆内存
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
	// 初始化需要用到的堆空间
	jsondata = new T;
	tempjArr = new T;
	//tempj = new T;
}

template<typename T, typename C>
void analysisJson<T, C>::analysisResponsedata(C* Gd)
{
	this->recursion(Gd->getExample(), Gd);//递归解析，并将结果传给回调函数
}


template <typename T,typename C>
void analysisJson<T, C>::recursion(T* jsondata,C* Gd) {

	// 遇数组则选第一个当作示例直接进入下一个递归
	if (jsondata->is_array()) {
		recursion(&((*jsondata)[0]) ,Gd);
	}
	else {

	
		structCall(Gd);// 调用一次回调函数

		for (auto& i : jsondata->items()) {

	
			tempj = &(i.value());// 临时 json 结构值发生变化
	

			if (jsondata->is_object()) {

				(*Gd->getVeclist()).push_back(i.key());// 往容器内添加一个成员，成员为当前 key

				if (tempj->is_string() || tempj->is_number_integer() || tempj->is_null()) {// 到达一个叶子（没有子成员的）节点

					structCall(Gd, true);// 调用一次回调函数
					Gd->getVeclist()->pop_back();// 弹出最近添加进容器的一个成员
				}

			}
			

			if (tempj->is_object())// 对象
			{
				recursion(tempj, Gd);
				if (!Gd->getVeclist()->empty()) {
					Gd->getVeclist()->pop_back();
				}
			}

			if (tempj->is_array())// 数组
			{
				//std::cout << "====================" << std::endl;
				//std::cout << (*tempj)[0].dump(4) << std::endl;
				//std::cout << Gd->getVeclist()->back() << std::endl;
				vectemp = *Gd->getVeclist();// 此处应该保留vector容器状态
				vectemp.pop_back();// 移除最新添加的一个成员
				recursion(&(*tempj)[0], Gd);// 递归数组第一个成员

				Gd->getVeclist()->clear();// 递归数组完成之后清除容器
				*Gd->getVeclist() = vectemp;// 恢复
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



