#pragma once
#include <assimp/Importer.hpp>
#include <DirectXMath.h>
#include <unordered_map>
#include <string>

struct aiNode;
struct aiNodeAnim;

class AnimationDataRow;
class TransformStructure;
class BoneStructure;

class FBXLoader
{
public:
	AnimationDataRow* loadAnimation(const char* path);


	BoneStructure* loadBone(const char* path);
	void createBoneStructure(aiNode* node);
	TransformStructure* createTransformFromData(aiNodeAnim* node);
	std::unordered_map<std::string, TransformStructure*> _transforms;

	DirectX::XMFLOAT3 toXMFLOAT3(aiVector3D vec);
	DirectX::XMFLOAT4 toXMFLOAT4(aiQuaternion quat);
};

