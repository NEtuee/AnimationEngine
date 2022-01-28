#pragma once
#include "Scene.h"
#include "DirectXDefine.h"
#include <vector>

class BoneStructure;
class TransformStructure;
class CharacterSet;
class AnimationLayerBase;
class Model;

class ModelScene : public Scene
{
public:
								ModelScene();
								~ModelScene();

	BoneStructure*				getBoneStructure();
	//CharacterSet*				getCharacter();

	void createGhosts(Graphics* graphics, std::vector<Scene*>& container, AnimationLayerBase* layer);

	void setWorldPosition(XMFLOAT3 world) { _worldPosition = world; }
	XMFLOAT3 getWorldPosition() { return _worldPosition; }
	void setBoneStructure(BoneStructure* bone);

	virtual void				createScene(Graphics*)			override;
	virtual void				destroyScene()					override;
	virtual void				frame(float)					override;
	virtual bool				renderScene(Graphics*, Camera*) override;
private:
	bool						renderBoneStructure(TransformStructure* root, Graphics*, Camera*);

private:
	XMFLOAT3					_worldPosition;

	Model*						_weaponModel;
	//CharacterSet*				_character;
	BoneStructure*				_boneStructure;
	TransformStructure*			_sword;
	TransformStructure*			_realSword;
	TransformStructure*			_shield;
};