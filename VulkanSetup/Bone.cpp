#include "Bone.h"
#include "TransformStructure.h"

Bone::Bone()
	:_name(""), _transform(new TransformStructure)
{
}

Bone::Bone(const Bone& target)
	: _name(target._name)
{
	_transform = new TransformStructure();
	_transform->SetWorldPosition(target._transform->getWorldPosition());
	_transform->SetWorldRotation(target._transform->getWorldRotation());
	_transform->SetWorldScale(target._transform->getWorldScale());
}

Bone::~Bone()
{
}

const std::string& Bone::getName()
{
	return _name;
}

const TransformStructure& Bone::getTransform()
{
	return (*_transform);
}
