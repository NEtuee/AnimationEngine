#include "FBXLoader.h"

#include <assert.h>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/anim.h>

#include "../VulkanSetup/TransformStructure.h"
#include "../VulkanSetup/Transform.h"
#include "../VulkanSetup/BoneStructure.h"
#include "../VulkanSetup/FrameData.h"
#include "../VulkanSetup/AnimationDataRow.h"
#include "../VulkanSetup/AnimationDataPack.h"
#include "../VulkanSetup/MathEx.h"


AnimationDataRow* FBXLoader::loadAnimation(const char* path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene)
	{
		assert(false);
	}

	if (scene->HasAnimations())
	{
		std::hash<std::string> hash;

		for (int i = 0; i < scene->mNumAnimations; ++i)
		{
			auto anim = scene->mAnimations[i];
			double duration = anim->mDuration;
			double fps = anim->mTicksPerSecond;
			AnimationDataRow* row = new AnimationDataRow;
			row->_fps = (short)fps;
			row->_boneConut = anim->mNumChannels;
			row->_boneScale = 1.f;
			row->_fileTag = "Check";
			row->_isAdditive = false;
			row->_isFacial = false;
			row->_isSinglebone = false;
			row->_hashBones.createSimpleHashtable(row->_boneConut);
			
			for (int j = 0; j < anim->mNumChannels; ++j)
			{
				auto bone = anim->mChannels[j];
				BoneDataRow boneData;
				boneData._name = bone->mNodeName.C_Str();

				for (int k = 0; k < bone->mNumPositionKeys; ++k)
				{
					auto key = bone->mPositionKeys[k];
					XMFLOAT3 scale = toXMFLOAT3(bone->mScalingKeys[k].mValue);
					XMFLOAT4 rotation = toXMFLOAT4(bone->mRotationKeys[k].mValue);
					XMFLOAT3 translation = MathEx::mul(toXMFLOAT3(key.mValue), 0.01f);

					FrameData data;
					data.frame = (int)key.mTime;
					data.setPosition(translation.x, translation.y, translation.z);
					data.setScale(scale.x, scale.y, scale.z);
					data.setRotation(rotation.x, rotation.y, rotation.z, rotation.w);
					data.updateVectors();

					boneData._frames.push_back(data);

					if (row->_frameCount < (short)data.frame + 1)
					{
						row->_frameCount = (short)data.frame + 1;
					}
				}

				row->_hashBones.pushData(hash(boneData._name), boneData);
				//row->_bones.emplace(hash(boneData._name), boneData);
				//std::cout << bone->mNumRotationKeys << ", " << bone->mNumPositionKeys << ", " << bone->mNumRotationKeys << "\n";
			}

			return row;
		}


	}
}

BoneStructure* FBXLoader::loadBone(const char* path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene)
	{
		assert(false);
	}

	
	
	//std::cout << scene->mRootNode[0]->c.C_Str();

	if (scene->HasAnimations())
	{
		_transforms.clear();
		for (int i = 0; i < scene->mAnimations[0]->mNumChannels; ++i)
		{
			if (scene->mAnimations[0]->mChannels[i]->mNodeName.length == 0)
				continue;

			auto transform = createTransformFromData(scene->mAnimations[0]->mChannels[i]);
			_transforms.emplace(transform->getName(), transform);
		}

		createBoneStructure(scene->mRootNode);
	}

	TransformStructure* rootBone = _transforms[scene->mAnimations[0]->mChannels[0]->mNodeName.C_Str()];
	rootBone->updateWorld();
	BoneStructure* bone = new BoneStructure(rootBone);
	//bone->createHashmap();
	return bone;
}

void FBXLoader::createBoneStructure(aiNode* node)
{
	//auto find = _transforms.find(node->mName.C_Str());
	//if (find == _transforms.end())
	//	return;

	std::cout << node->mName.C_Str() << "\n";

	for (int i = 0; i < node->mNumChildren; ++i)
	{
		auto find = _transforms.find(node->mChildren[i]->mName.C_Str());
		if (find != _transforms.end())
		{
			TransformStructure* child = _transforms[node->mName.C_Str()];
			if (child != nullptr)
			{
				Transform local = child->getWorldTransform();
				(*find).second->setParent(child);
				child->SetLoaclTransformNoCalc(local.getPosition(), local.getScale(), local.getRotation());
			}

		}

		//(*find).second->setParent(_transforms[node->mName.C_Str()]);

		createBoneStructure(node->mChildren[i]);
	}
}

TransformStructure* FBXLoader::createTransformFromData(aiNodeAnim* node)
{
	XMFLOAT3 scale = toXMFLOAT3(node->mScalingKeys[0].mValue);
	XMFLOAT4 rotation = toXMFLOAT4(node->mRotationKeys[0].mValue);
	XMFLOAT3 translation = MathEx::mul(toXMFLOAT3(node->mPositionKeys[0].mValue),0.01f);

	//MathEx::swap<float>(translation.z, translation.y);
	//MathEx::swap<float>(rotation.z, rotation.y);

	std::hash<std::string> nameHash;

	TransformStructure* transform = new TransformStructure;
	transform->setName(node->mNodeName.C_Str());
	transform->setHashedName(nameHash(transform->getName()));
	transform->SetWorldPosition(translation);
	transform->SetWorldRotation(XMVectorSet(rotation.x, rotation.y, rotation.z, rotation.w));
	transform->SetWorldScale(scale);
	
	return transform;

	//return nullptr;
}

DirectX::XMFLOAT3 FBXLoader::toXMFLOAT3(aiVector3D vec)
{
	return DirectX::XMFLOAT3{ vec.x, vec.y ,vec.z};
}

DirectX::XMFLOAT4 FBXLoader::toXMFLOAT4(aiQuaternion quat)
{
	return DirectX::XMFLOAT4{ quat.x,quat.y,quat.z,quat.w };
}
