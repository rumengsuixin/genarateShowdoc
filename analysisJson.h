#pragma once
#include <json.hpp>
#include <fifo_map.hpp>

// callback

using callback = void(*) (void* arg);

using namespace nlohmann;

// A workaround to give to use fifo_map as map, we are just ignoring the 'less' compare
template<class K, class V, class dummy_compare, class A>
using my_workaround_fifo_map = fifo_map<K, V, fifo_map_compare<K>, A>;
using my_json = basic_json<my_workaround_fifo_map>;




template <typename T,typename C>
class analysisJson {
public:
	analysisJson(void);
	analysisJson(json jsonobj);
	analysisJson(my_json jsonobj);
	analysisJson(std::string jsontext);
	analysisJson(std::ifstream* stream);
	~analysisJson(void);

	void modify(std::string jsontext);
	T* getJson(void);
	// reslove 
	//template <typename C>
	//C reslove(void);

	// configure
	void example(void);


	// recursion
	void recursion(T* jsondata, std::vector<std::string>* veclist,C* Gd, callback cb);
	void recursion(T* jsondata);

	void WriteIn(std::vector<std::string>* veclist, std::string key, T& ison,C* Gd, callback cb);



private:
	T* jsondata;
	T* tempjArr;
	void initialization(void);


};


