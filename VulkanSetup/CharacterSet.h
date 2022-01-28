#pragma once
#include <string>
#include <unordered_map>

class BoneStructure;
class BlendTree;
class AnimationDataPack;
class CharacterSetLoader;

class CharacterSet
{
public:
						CharacterSet();

	void				createCharacterSet(std::string name);
	void				deleteCharacterSet();

	void				frame(float deltaTime);

	void				setName(std::string name);

	BoneStructure*		getBoneStructure();
	const BlendTree*	getBlendTree();
	const std::string&	getName();
	const std::unordered_map<std::string, AnimationDataPack*>& getAnimations();
private:
	//std::unordered_map<size_t, AnimationDataPack*> _animations;
	std::unordered_map<std::string, AnimationDataPack*> _animations;
	std::string			_name;
	BoneStructure*		_bone;
	BlendTree*			_tree;

	friend CharacterSetLoader;
};

