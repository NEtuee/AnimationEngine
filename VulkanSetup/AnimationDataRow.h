#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <time.h>
#include "Serialize.h"
#include "DirectXDefine.h"
#include "FrameData.h"
#include "SimpleHashtable.h"

const extern char positionFlag;
const extern char scaleFlag;
const extern char rotationFlag;


struct BoneDataRow
{
	std::string		_name;
	std::vector<FrameData> _frames;


	void serialize(Serialization* serialize, std::ostream* stream);
	void deserialize(Serialization* serialize, std::istream* stream);
};

struct AnimationDataRow
{
	float			_boneScale		= 0.f;

	short			_boneConut		= 0;
	short			_frameCount		= 0;
	short			_fps			= 0;

	bool			_isFacial		= false;
	bool			_isAdditive		= false;
	bool			_isSinglebone	= false;

	std::string		_fileTag		= "";
	//bone info, name hash
	//std::unordered_map<size_t, BoneDataRow> _bones;
	SimpleHashtable<BoneDataRow> _hashBones;

	void serialize(Serialization* serialize, std::ostream* stream);
	void deserialize(Serialization* serialize, std::istream* stream);
};

