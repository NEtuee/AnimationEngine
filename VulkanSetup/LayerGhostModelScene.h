#pragma once
#include "ModelScene.h"

class AnimationSourceLayer;
class LayerGhostModelScene : public ModelScene
{
public:
	void setLayer(AnimationSourceLayer* layer);

	virtual void destroyScene() override;
	virtual void frame(float deltaTime) override;
private:
	void setAnimation(TransformStructure* root);
private:
	AnimationSourceLayer* _targetLayer;
};

