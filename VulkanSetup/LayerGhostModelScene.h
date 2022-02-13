#pragma once
#include "ModelScene.h"
#include <vector>

class AnimationLayerBase;
class BoneStructure;
class TransformStructure;
class CharacterSet;
class AnimationLayerBase;
class Model;
class IKChain;
class LookAtIKSolver;

class LayerGhostModelScene : public ModelScene
{
public:
							LayerGhostModelScene();

	void					setLayer(AnimationLayerBase* layer);
	void					setBoneStructure(BoneStructure* bone);
	void					setIKSolver(LookAtIKSolver* solver);

	virtual void			createScene(Graphics*)			override;
	virtual void			destroyScene() override;
	virtual void			frame(float deltaTime) override;
	virtual bool			renderScene(Graphics*, Camera*) override;
private:
	void					setAnimation();
private:
	AnimationLayerBase*		_targetLayer;
	IKChain*				_chain;
};

