#include "LayerGhostModelScene.h"
#include "AnimationSourceLayer.h"
#include "AnimationLayerBase.h"
#include "Transform.h"
#include "TransformStructure.h"
#include "BoneStructure.h"

#include "Camera.h"
#include "Model.h"
#include "Graphics.h"
#include "Shader.h"

#include "ThreadPool.h"
#include "IKChain.h"
#include "LookAtIKSolver.h"

LayerGhostModelScene::LayerGhostModelScene()
:_targetLayer(nullptr), _chain(nullptr)
{
}

void LayerGhostModelScene::setLayer(AnimationLayerBase* layer)
{
	_targetLayer = layer;
}

void LayerGhostModelScene::setBoneStructure(BoneStructure* bone)
{
	ModelScene::setBoneStructure(new BoneStructure(*bone));
}

void LayerGhostModelScene::setIKSolver(LookAtIKSolver* solver)
{
	_chain = new IKChain;
	LookAtIKSolver* lookat = new LookAtIKSolver;
	
	auto bone = getBoneStructure();
	lookat->setOptionBone(bone->find(solver->getHead()->getName()), bone->find(solver->getLookForward()->getName()));
	_chain->createIKChain(bone->find(solver->getEffector()->getName()), bone->find(solver->getRoot()->getName()), solver->getTarget(), lookat);
}

void LayerGhostModelScene::createScene(Graphics* graphics)
{
	auto bone = getBoneStructure();
	Model* model = graphics->createCube(XMFLOAT4(1.f, 0.3f, 1.0f, 0.3f));
	model->setRenderLine(true);
	bone->setModel(model);
	bone->setShader(graphics->createBaseShader());
	bone->setModelScale(0.01f);
}

void LayerGhostModelScene::destroyScene()
{
	getBoneStructure()->getModel()->destroyModel();
	delete getBoneStructure()->getModel();
	getBoneStructure()->getShader()->destroyShader();
	delete getBoneStructure()->getShader();
	getBoneStructure()->destroyBoneStructure();
	delete getBoneStructure();
}

void LayerGhostModelScene::frame(float deltaTime)
{
	setAnimation();
	if(_chain != nullptr)
		_chain->solve();

	//ThreadPool::getInstance()->enqueueJob([](LayerGhostModelScene* ghost, IKChain* chain)
	//	{
	//		ghost->setAnimation();
	//		if(chain != nullptr)
	//			chain->solve();
	//	}, this, _chain);
}

bool LayerGhostModelScene::renderScene(Graphics* graphics, Camera* camera)
{
	auto bone = getBoneStructure();
	graphics->renderBegin(bone->getModel());

	renderBoneStructure(&bone->getRootBone(), graphics, camera);

	return true;
}

void LayerGhostModelScene::setAnimation()
{
	auto& order = getBoneStructure()->getUpdateOrder();
	float depth = 0.f;
	bool masking = false;

	for (int i = 0; i < order.size(); ++i)
	{
		TransformStructure* transform = order[i];
		if (depth >= transform->getDepth())
			masking = false;

		size_t outIndex;
		Transform pose = _targetLayer->getCurrentPose(transform, outIndex, masking);
		if (outIndex != -1)
			transform->SetLoaclTransformNoCalc(pose.getPosition(), pose.getScale(), pose.getRotation());
		if (outIndex == -2)
		{
			depth = transform->getDepth();
			masking = true;
		}
	}

	getBoneStructure()->getRootBone().updateWorld();

	//int outindex;
	//Transform tp = _targetLayer->getCurrentPose(root, outindex,masking);

	//if (outindex != -1)
	//{
	//	root->SetLoaclTransformNoCalc(tp.getPosition(),tp.getScale(),tp.getRotation());
	//}
	//if (outindex == -2)
	//	masking = true;

	//auto& children = root->getChildren();
	//for (auto iter = children.begin(); iter != children.end(); ++iter)
	//{
	//	setAnimation(*iter, masking);
	//}

	//_targetLayer->afterLoop(root);
}