#pragma once
#include <vector>
#include <unordered_map>
#include <string>

class Model;
class Shader;
class TransformStructure;
class BoneStructure
{
public:
								BoneStructure(TransformStructure* root);
								BoneStructure(const BoneStructure&);
								~BoneStructure();
	
	void						createHashmap();
	void						createUpdateOrder();
	void						destroyBoneStructure();

	void						setModel(Model* model);
	void						setModelScale(float scale);
	void						setShader(Shader* shader) { _shader = shader; }

	TransformStructure*			getStructureTransform() { return _structureTransform; }
	TransformStructure&			getRootBone();
	TransformStructure*			find(const std::string target);
	Model*						getModel();
	Shader*						getShader() { return _shader; }
	float						getModelScale() { return _boneModelScale; }

	void						addToHashmap(TransformStructure* target, float depth);

	const std::vector<TransformStructure*>& getUpdateOrder() { return _transformUpdateOrder; }
private:
	//TransformStructure*			findBone(const std::string target, TransformStructure* root);

	void						createHashmap(TransformStructure* root, float depth);
	void						createUpdateOrder(TransformStructure* root);
	void						destroyBoneTransforms(TransformStructure* root);
private:
	TransformStructure*			_structureTransform = nullptr;
	TransformStructure*			_rootBone = nullptr;

	Model*						_model = nullptr;
	Shader*						_shader = nullptr;

	std::unordered_map<size_t,TransformStructure*> _hashedMap;
	std::vector<TransformStructure*> _transformUpdateOrder;

	int							_boneModelCount = 0;
	int							_updateCount = 0;
	float						_boneModelScale = 1.f;
};

