#include "LayerGhostModelScene.h"
#include "AnimationSourceLayer.h"
#include "Transform.h"
#include "TransformStructure.h"
#include "BoneStructure.h"

void LayerGhostModelScene::setLayer(AnimationSourceLayer* layer)
{
	_targetLayer = layer;
}

void LayerGhostModelScene::destroyScene()
{
}

void LayerGhostModelScene::frame(float deltaTime)
{
	setAnimation(&getBoneStructure()->getRootBone());
}

void LayerGhostModelScene::setAnimation(TransformStructure* root)
{
	int outindex;
	Transform tp = _targetLayer->getCurrentPose(root, outindex);

	if (outindex != -1)
	{
		root->SetLoaclTransformNoCalc(tp.getPosition(),tp.getScale(),tp.getRotation());
	}

	auto& children = root->getChildren();
	for (auto iter = children.begin(); iter != children.end(); ++iter)
	{
		setAnimation(*iter);
	}
}
