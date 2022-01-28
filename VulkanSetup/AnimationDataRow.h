#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <time.h>
#include "Serialize.h"
#include "DirectXDefine.h"
#include "FrameData.h"

const extern char positionFlag;
const extern char scaleFlag;
const extern char rotationFlag;


struct BoneDataRow
{
	std::string		_name;
	std::vector<FrameData> _frames;


	void serialize(Serialization* serialize, std::ostream* stream)
	{
		serialize->writeString(stream, _name);
		serialize->writeVector(stream, _frames);
	}

	void deserialize(Serialization* serialize, std::istream* stream)
	{
		serialize->readString(stream, _name);
		serialize->readVector(stream, _frames);
	}
};

struct AnimationDataRow
{
	float			_boneScale;

	short			_boneConut;
	short			_frameCount;
	short			_fps;

	bool			_isFacial;
	bool			_isAdditive;
	bool			_isSinglebone;

	std::string		_fileTag;
	//bone info, name hash
	std::unordered_map<size_t, BoneDataRow> _bones;

	void serialize(Serialization* serialize, std::ostream* stream)
	{
		clock_t start, end;
		double result;
		start = clock();

		serialize->write(stream, &_boneScale,		sizeof(_boneScale));
		serialize->write(stream, &_frameCount,		sizeof(_frameCount));
		serialize->write(stream, &_boneConut,		sizeof(_boneConut));
		serialize->write(stream, &_fps,				sizeof(_fps));
		serialize->write(stream, &_isFacial,		sizeof(_isFacial));
		serialize->write(stream, &_isAdditive,		sizeof(_isAdditive));
		serialize->write(stream, &_isSinglebone,	sizeof(_isSinglebone));
		
		serialize->writeString(stream, _fileTag);
		serialize->writeUnMap(stream, _bones);

		end = clock() - start;
		result = static_cast<double>(end);
		std::cout << "animation serialization : " << result << "ms" << std::endl;
	}

	void deserialize(Serialization* serialize, std::istream* stream)
	{
		clock_t start, end;
		double result;
		start = clock();

		//serialize->read(stream, );
		serialize->read(stream, &_boneScale,	sizeof(_boneScale));
		serialize->read(stream, &_frameCount,	sizeof(_frameCount));
		serialize->read(stream, &_boneConut,	sizeof(_boneConut));
		serialize->read(stream, &_fps,			sizeof(_fps));
		serialize->read(stream, &_isFacial,		sizeof(_isFacial));
		serialize->read(stream, &_isAdditive,	sizeof(_isAdditive));
		serialize->read(stream, &_isSinglebone, sizeof(_isSinglebone));


		serialize->readString(stream, _fileTag);
		serialize->readUnMap(stream, _bones);

		end = clock() - start;
		result = static_cast<double>(end);
		std::cout << "animation deserialization : " << result << "ms" << std::endl;
	}
};

