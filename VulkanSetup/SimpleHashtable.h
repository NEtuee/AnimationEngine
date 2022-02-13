#pragma once
#include <vector>

template<typename T>
struct TableData
{
	size_t _key;
	T _data;
};

template<typename T>
class SimpleHashtable
{
public:
	void createSimpleHashtable(size_t size);
	void destroySimpleHashtable();
	bool find(size_t key, T*& result);
	
	size_t size() { return _size; }
	size_t dataCount() { return _dataCount; }

	void pushData(size_t key, T data);

	std::vector<std::vector<TableData<T>>>& getTable() {return _data; }
private:
	size_t toIndex(size_t key);
private:
	std::vector<std::vector<TableData<T>>> _data;
	size_t _size = 0;
	size_t _dataCount = 0;
};

template<typename T>
inline void SimpleHashtable<T>::createSimpleHashtable(size_t size)
{
	_data.resize(size);
	_size = size;
}

template<typename T>
inline void SimpleHashtable<T>::destroySimpleHashtable()
{
	for (size_t i = 0; i < _size; ++i)
	{
		if (_data[i])
			_data[i].clear();
	}
	
	_data.clear();
}

template<typename T>
inline bool SimpleHashtable<T>::find(size_t key, T*& result)
{
	size_t index = toIndex(key);
	for (size_t i = 0; i < _data[index].size(); ++i)
	{
		if (_data[index][i]._key == key)
		{
			result = &_data[index][i]._data;
			return true;
		}
			
	}

	return false;
}

template<typename T>
inline void SimpleHashtable<T>::pushData(size_t key, T data)
{
	size_t index = toIndex(key);
	
	_data[index].push_back(TableData<T>{key, data});
	++_dataCount;
	/*for (int i = 0; i < _data[index].size(); ++i)
	{
		if()
	}*/
	//for()
}

template<typename T>
inline size_t SimpleHashtable<T>::toIndex(size_t key)
{
	return key % _size;
}
