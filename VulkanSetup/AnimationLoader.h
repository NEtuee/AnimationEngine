#pragma once
#include <string>
#include "DirectXDefine.h"

struct AnimationDataRow;

class AnimationDataPack;
class TransformStructure;
class XMLElement;

class AnimationLoader
{
public:
	void				optimization(AnimationDataRow& data);
	AnimationDataPack*	loadDataPack(std::string path);
	AnimationDataRow*	loadAnimation(std::string path);
	AnimationDataRow*	loadAnimationBinary(std::string path);
};

