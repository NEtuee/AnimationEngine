#pragma once
#include <string>
#include <vector>
#include <unordered_map>


enum class StateVariableTypeEnum;

class AnimationLayerBase;
class AnimationStateVariable;
class BoneStructure;
class TransformStructure;
class Transform;

class BlendTree
{
public:
									BlendTree();	

	void							createBlendTree(BoneStructure* bone);
	void							destroyBlendTree();

	void							initialize();

	void							frame(float deltaTime);

	AnimationStateVariable*			createVariable(StateVariableTypeEnum type, std::string name, void* initialValue);

	void							setRootMotion(bool enable, TransformStructure* rootBone);

	AnimationLayerBase* getLayer() const;
	void setResultLayer(AnimationLayerBase* layer);
	const std::unordered_map<std::string, AnimationStateVariable*>& getVariables() const;
	AnimationStateVariable*			getVariable(std::string name);
private:
	void							playAnimation(TransformStructure* root);
	void							updateRootMotion();
private:
	AnimationLayerBase* _resultLayer;
	std::unordered_map<std::string, AnimationStateVariable*>	_variables;

	BoneStructure*												_targetBone;

	Transform*													_prevRoot;
	TransformStructure*											_rootBone;
	bool														_rootMotion;
};

