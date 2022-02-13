#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>
#include "SimpleHashtable.h"

class Serialization
{
public:
	inline void write(std::ostream* os, const void* v, size_t len)
	{
		if (!os->write(reinterpret_cast<const char*>(v), len))
		{
			assert(false);
		}
	}

	inline void writeString(std::ostream* os, const std::string& target)
	{
		std::string::size_type length = target.length();
		write(os, &length, sizeof(length));
		write(os, target.data(), length);
	}

	template<class T>
	inline void writeVector(std::ostream* os, typename std::vector<T>& target)
	{
		size_t size = target.size();
		write(os, &size, sizeof(size));

		for (auto iter = target.begin(); iter != target.end(); ++iter)
		{
			(*iter).serialize(this, os);
		}
	}

	template<class c1, class c2>
	inline void writeUnMap(std::ostream* os, typename std::unordered_map<c1, c2>& target)
	{
		size_t size = target.size();
		write(os, &size, sizeof(size));

		for (auto iter = target.begin(); iter != target.end(); ++iter)
		{
			writePair(os, (*iter));
		}
	}

	template<class c1>
	inline void writeUnMap(std::ostream* os, typename SimpleHashtable<c1>& target)
	{
		size_t size = target.dataCount();
		write(os, &size, sizeof(size));


		auto& table = target.getTable();
		for (int i = 0; i < target.size(); ++i)
		{
			for (int j = 0; j < table[i].size(); ++j)
			{
				write(os, &table[i][j]._key, sizeof(size_t));
				table[i][j]._data.serialize(this, os);
			}
		}
		//for (auto iter = target.begin(); iter != target.end(); ++iter)
		//{
		//	writePair(os, (*iter));
		//}
	}

	template<class c1, class c2>
	inline void writePair(std::ostream* os, typename std::pair<c1, c2>& target)
	{
		write(os, &target.first, sizeof(target.first));
		target.second.serialize(this, os);
		//write(os, target.second);
	}



	inline void read(std::istream* is, void* v, size_t len)
	{
		if (!is->read(reinterpret_cast<char*>(v), len))
		{
			assert(false);
		}
	}

	inline void readString(std::istream* is, typename std::string& target)
	{
		std::string::size_type length;
		read(is, &length, sizeof(length));

		target.resize(length);
		read(is, static_cast<void*>(target.data()), length);
	}

	template<class T>
	inline void readVector(std::istream* is, typename std::vector<T>& target)
	{
		size_t size = target.size();
		read(is, &size, sizeof(size));

		target.resize(size);

		for (auto iter = target.begin(); iter != target.end(); ++iter)
		{
			(*iter).deserialize(this, is);
		}
	}

	template<class c1, class c2>
	inline void readUnMap(std::istream* is, typename std::unordered_map<c1, c2>& target)
	{
		size_t size;
		read(is, &size, sizeof(size));
		target.clear();

		for (int i = 0; i < size; ++i)
		{
			std::pair<c1, c2> pair;
			readPair<c1, c2>(is, pair);
			target.emplace(pair);

		}
	}

	template<class c1>
	inline void readUnMap(std::istream* is, typename SimpleHashtable<c1>& target)
	{
		size_t size;
		read(is, &size, sizeof(size));

		target.createSimpleHashtable(size);
		//target.clear();

		for (size_t i = 0; i < size; ++i)
		{
			std::pair<size_t, c1> pair;
			readPair<size_t, c1>(is, pair);
			//target.emplace(pair);
			if (i == 83)
				int e = 0;
			target.pushData(pair.first, pair.second);
		}
	}

	template<class c1, class c2>
	inline void readPair(std::istream* is, typename std::pair<c1, c2>& target)
	{
		read(is, &target.first, sizeof(c1));
		target.second.deserialize(this, is);
	}

};


