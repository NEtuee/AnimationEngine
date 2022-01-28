#pragma once
#include <string>

class TransformStructure;
class Bone
{
public:
								Bone();
								Bone(const Bone&);
								~Bone();

	const std::string&			getName();
	const TransformStructure&	getTransform();
private:
	std::string					_name;
	TransformStructure*			_transform;
};

