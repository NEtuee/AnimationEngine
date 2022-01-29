#pragma once
#include <string>
#include "DirectXDefine.h"

class TransformStructure;
class XMLElement;

class SkeletonLoader
{
public:
	TransformStructure*		createTransformFromData(XMLElement* data);
	TransformStructure*		loadSkeleton(std::string path, std::string renderTarget);
	TransformStructure*		loadSkeletonBinary(std::string path);
};


