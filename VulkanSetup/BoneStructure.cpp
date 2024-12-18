﻿#include "BoneStructure.h"
#include "TransformStructure.h"
#include "Model.h"
#include <math.h>

BoneStructure::BoneStructure(TransformStructure* root)
{
	_structureTransform = new TransformStructure;
	_structureTransform->setName("Structure");
	_rootBone = root;
	std::hash<std::string> nameHash;
	_rootBone->setHashedName(nameHash(_rootBone->getName()));
	_rootBone->setParent(_structureTransform);
	createHashmap();
	createUpdateOrder();
}

BoneStructure::BoneStructure(const BoneStructure& bone)
{
	_structureTransform = new TransformStructure;
	_structureTransform->setName("Structure");
	_rootBone = bone._rootBone->copyThis();
	_rootBone->setParent(_structureTransform);
	createHashmap();
	createUpdateOrder();
}

BoneStructure::~BoneStructure()
{
}

void BoneStructure::destroyBoneStructure()
{
	delete _structureTransform;
	destroyBoneTransforms(_rootBone);

}

void BoneStructure::setModel(Model* model)
{
	_model = model;
}

void BoneStructure::setModelScale(float scale)
{
	_boneModelScale = scale;
}

TransformStructure& BoneStructure::getRootBone()
{
	return *_rootBone;
}

TransformStructure* BoneStructure::find(const std::string target)
{
	std::hash<std::string> hash;
	auto iter = _hashedMap.find(hash(target));
	if (iter != _hashedMap.end())
		return (*iter).second;

	return nullptr;
	//return *findBone(target, _rootBone);
}

Model* BoneStructure::getModel()
{
	return _model;
}

void BoneStructure::addToHashmap(TransformStructure* target, float depth)
{
	_hashedMap.insert(std::make_pair(target->getHashedName(), target));
	target->setDepth(depth);
}

//TransformStructure* BoneStructure::findBone(const std::string target, TransformStructure* root)
//{
//	if (root->getName().compare(target) == 0)
//		return root;
//
//	auto children = root->getChildren();
//	for (auto iterator = children.begin(); iterator != children.end(); ++iterator)
//	{
//		TransformStructure* find = findBone(target, (*iterator));
//		if (find != nullptr)
//			return find;
//	}
//
//	return nullptr;
//}

void BoneStructure::createHashmap()
{
	_hashedMap.clear();
	createHashmap(_rootBone,0.f);
}

void BoneStructure::createUpdateOrder()
{
	_transformUpdateOrder.clear();
	createUpdateOrder(_rootBone);
}

void BoneStructure::createHashmap(TransformStructure* root, float depth)
{
	_hashedMap.insert(std::make_pair(root->getHashedName(), root));
	root->setDepth(depth);

	auto& children = root->getChildren();
	for (auto iterator = children.begin(); iterator != children.end(); ++iterator)
	{
		createHashmap(*iterator, depth + 1.f);
	}
}

void BoneStructure::createUpdateOrder(TransformStructure* root)
{
	_transformUpdateOrder.push_back(root);

	auto& children = root->getChildren();
	for (auto iterator = children.begin(); iterator != children.end(); ++iterator)
	{
		createUpdateOrder(*iterator);
	}

	
}

void BoneStructure::destroyBoneTransforms(TransformStructure* root)
{
	auto children = root->getChildren();
	for (auto iterator = children.begin(); iterator != children.end(); ++iterator)
	{
		destroyBoneTransforms((*iterator));
	}

	delete root;
}