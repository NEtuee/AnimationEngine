#pragma once
#include "Scene.h"
#include "DirectXDefine.h"
#include <vector>

class BoneStructure;
class TransformStructure;
class CharacterSet;
class AnimationLayerBase;
class LayerGhostModelScene;
class Model;

class ModelScene : public Scene
{
public:
								ModelScene();
								~ModelScene();

	BoneStructure*				getBoneStructure();

	void						createGhosts(Graphics* graphics, std::vector<LayerGhostModelScene*>& container, AnimationLayerBase* layer);

	void						setWorldPosition(XMFLOAT3 world) { _worldPosition = world;}
	XMFLOAT3					getWorldPosition() { return _worldPosition; }
	void						setBoneStructure(BoneStructure* bone);
	void						setLayer(AnimationLayerBase* layer);

	virtual void				createScene(Graphics*)			override;
	virtual void				destroyScene()					override;
	virtual void				frame(float)					override;
	virtual bool				renderScene(Graphics*, Camera*) override;
protected:
	bool						renderBoneStructure(TransformStructure* root, Graphics*, Camera*);
	bool						renderBoneStructureForward(TransformStructure* root, Graphics*, Camera*);

private:
	XMFLOAT3					_worldPosition;

	AnimationLayerBase*			_layer;
	Model*						_weaponModel;
	//CharacterSet*				_character;
	BoneStructure*				_boneStructure;
	TransformStructure*			_sword;
	TransformStructure*			_realSword;
	TransformStructure*			_shield;

	std::vector<LayerGhostModelScene*> _ghosts;
};