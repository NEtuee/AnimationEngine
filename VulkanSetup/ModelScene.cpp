#include "ModelScene.h"
#include "TransformStructure.h"
#include "BoneStructure.h"
#include "CharacterSet.h"

#include "Camera.h"
#include "Model.h"
#include "Graphics.h"
#include "Shader.h"

#include "AnimationDataRow.h"
#include "AnimationSourceLayer.h"
#include "AnimationBlendLayer.h"

#include "ResourceLoader.h"
#include "CharacterSetLoader.h"

#include "MathEx.h"
#include "LayerGhostModelScene.h"

#include <filesystem>

ModelScene::ModelScene()
	:/*_character(nullptr),*/ _worldPosition{0.f,0.f,0.f}, _weaponModel(nullptr), _sword(nullptr), _realSword(nullptr), _shield(nullptr)
{
}

ModelScene::~ModelScene()
{
}

BoneStructure* ModelScene::getBoneStructure()
{
	return _boneStructure;
}
static float x = 0.f;
void ModelScene::createGhosts(Graphics* graphics, std::vector<LayerGhostModelScene*>& container, AnimationLayerBase* layer)
{
	AnimationBlendLayer* blendLayer = dynamic_cast<AnimationBlendLayer*>(layer);
	if (blendLayer != nullptr)
	{
		createGhosts(graphics,container,blendLayer->getLayerOne());
		createGhosts(graphics,container, blendLayer->getLayerTwo());
	}
	else
	{
		LayerGhostModelScene* ghost = new LayerGhostModelScene;
		ghost->setLayer(static_cast<AnimationSourceLayer*>(layer));
		ghost->setBoneStructure(_boneStructure);
		ghost->createScene(graphics);

		ghost->setWorldPosition(XMFLOAT3(++x, 0.f, 0.f));
		_ghosts.push_back(ghost);
		//ghost->createScene()
	}
}

void ModelScene::setBoneStructure(BoneStructure* bone)
{
	_boneStructure = bone;
}

void ModelScene::setLayer(AnimationLayerBase* layer)
{
	_layer = layer;
}

void ModelScene::createScene(Graphics* graphics)
{
	auto bone = _boneStructure;
	Model* model = graphics->createCube(XMFLOAT4( 1.f,0.3f,0.3f,1.f ));
	model->setRenderLine(true);
	bone->setModel(model);
	bone->setShader(graphics->createBaseShader());
	bone->setModelScale(0.01f);

	_weaponModel = graphics->createCube(XMFLOAT4(0.f, 0.6f, 0.3f, 1.f));
	_weaponModel->setRenderLine(false);

	_sword = new TransformStructure;
	_realSword = new TransformStructure;

	_sword->setName("Sword");
	_sword->setParent(_boneStructure->find("Bip_Weapon_R"));
	_sword->SetLocalScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	_sword->SetLocalPosition(XMFLOAT3(0.f, 0.f, 0.f));
	_sword->SetLocalRotation(XMQuaternionIdentity());

	_realSword->setName("RealSword");
	_realSword->setParent(_sword);
	_realSword->SetLocalScale(XMFLOAT3(1.0f, 0.02f, 0.04f));
	_realSword->SetLocalPosition(XMFLOAT3(0.35f, 0.f, 0.f));
	_realSword->SetLocalRotation(XMQuaternionIdentity());

	_shield = new TransformStructure;
	_shield->setParent(_boneStructure->find("Bip_Shield_L"));
	_shield->SetLocalPosition(XMFLOAT3(0.f, 0.f, 0.f));
	_shield->SetLocalScale(XMFLOAT3(.38f, 0.02f, .38f));
	_shield->SetLocalRotation(XMQuaternionIdentity());

	createGhosts(graphics, _ghosts, _layer);
}

void ModelScene::destroyScene()
{
	auto bone = _boneStructure;
	bone->getModel()->destroyModel();
	delete bone->getModel();

	_weaponModel->destroyModel();
	delete _weaponModel;

	bone->getShader()->destroyShader();
	delete bone->getShader();

	for (auto iter = _ghosts.begin(); iter != _ghosts.end(); ++iter)
	{
		(*iter)->destroyScene();
	}
	_ghosts.clear();
	x = 0.f;
	//bone->destroyBoneStructure();
	//delete bone;
}

void ModelScene::frame(float deltaTime)
{
/*	static float x = 0.f;
	static float y = 0.f;
	x += deltaTime * .5f;
	y += deltaTime * .5f;

	_boneStructure->find("Hand IK Target")->SetWorldPosition(XMVectorSet(4.f + cosf(x) * 8.f, 1.3f, 4.f + sinf(y) * 8.f, 0.f));*/

	for (auto iter = _ghosts.begin(); iter != _ghosts.end(); ++iter)
	{
		(*iter)->frame(deltaTime);
	}
}

bool ModelScene::renderScene(Graphics* graphics, Camera* camera)
{
	auto bone = _boneStructure;
	graphics->renderBegin(bone->getModel());

	renderBoneStructure(&bone->getRootBone(), graphics, camera);


	graphics->renderBegin(_weaponModel);
	//renderBoneStructureForward(_boneStructure->find("Bip001 Head"), graphics, camera);
	//renderBoneStructureForward(_boneStructure->find("Bip001 Neck"), graphics, camera);
	//renderBoneStructureForward(_boneStructure->find("Bip001 Spine2"), graphics, camera);
	//renderBoneStructureForward(_boneStructure->find("Bip001 Spine1"), graphics, camera);
	//renderBoneStructureForward(_boneStructure->find("Bip001 Spine"), graphics, camera);



	auto targetBone = _boneStructure->find("Bip001 Head");
	XMMATRIX matrix = XMMatrixScaling(0.1f, 0.3f, 0.1f);
	matrix *= XMMatrixRotationQuaternion(targetBone->getWorldRotation());
	matrix *= XMMatrixTranslationFromVector(targetBone->getWorldPosition());
	if (graphics->renderModelToShader(bone->getShader(), _weaponModel, matrix, camera) == false)
	{
		return false;
	}

	for (auto iter = _ghosts.begin(); iter != _ghosts.end(); ++iter)
	{
		(*iter)->renderScene(graphics, camera);
	}
	/*if (graphics->renderModelToShader(bone->getShader(), _weaponModel, _realSword->getMatrix(), camera) == false)
	{
		return false;
	}
	if (graphics->renderModelToShader(bone->getShader(), _weaponModel, _shield->getMatrix(), camera) == false)
	{
		return false;
	}*/

	return true;
}

bool ModelScene::renderBoneStructure(TransformStructure* root, Graphics* graphics, Camera* camera)
{
	if (root->isCanDraw() == false)
		return true;

	auto& children = root->getChildren();
	if (children.size() == 0)
		return true;

	for (auto iterator = children.begin(); iterator != children.end(); ++iterator)
	{
		if (renderBoneStructure((*iterator), graphics, camera) == false)
			return false;

		if((*iterator)->isCanDraw() == false)
			continue;

		XMVECTOR dir = root->getWorldPosition() - (*iterator)->getWorldPosition();
		XMVECTOR len = XMVector3Length(dir);
		XMFLOAT3 flen;
		XMStoreFloat3(&flen, len);
		dir = XMVector3Normalize(dir);

		XMVECTOR forward = XMVectorSet(0.f, 0.f, 1.f, 0.f);
		XMFLOAT3 dot;
		XMStoreFloat3(&dot, XMVector3Dot(forward, dir));
		float angle = static_cast<float>(acos(dot.x));
		XMVECTOR rotAxis = XMVector3Normalize(XMVector3Cross(forward, dir));
		XMVECTOR quaternion;

		if (XMVector3Equal(rotAxis, XMVectorZero()))
		{
			quaternion = XMQuaternionIdentity();
		}
		else
		{
			quaternion = XMQuaternionRotationAxis(rotAxis, angle);
		}

		XMMATRIX world;
		
		auto bone = _boneStructure;
		world = XMMatrixScaling(bone->getModelScale(), bone->getModelScale(), flen.x);
		world *= XMMatrixRotationQuaternion(quaternion);
		world *= XMMatrixTranslationFromVector((*iterator)->getWorldPosition() + dir * (len * 0.5f) + 
			XMVectorSet(_worldPosition.x, _worldPosition.y, _worldPosition.z,0.f));

		if (graphics->renderModelToShader(bone->getShader(), bone->getModel(), world, camera) == false)
			return false;
	}

	return true;
}

bool ModelScene::renderBoneStructureForward(TransformStructure* root, Graphics* graphics, Camera* camera)
{
	if (root->isCanDraw() == false)
		return true;

	if (root->isCanDraw() == false)
		return true;

	XMVECTOR quat = MathEx::fromToQuaternion(XMVectorSet(0.f, 0.f, 1.f, 0.f), root->getWorldTransform().getUp());

	XMMATRIX world;

	auto bone = _boneStructure;
	world = XMMatrixScaling(bone->getModelScale(), bone->getModelScale(), .6f);
	world *= XMMatrixRotationQuaternion(quat);
	world *= XMMatrixTranslationFromVector(root->getWorldPosition() + root->getWorldTransform().getUp() * 0.3f +
		XMVectorSet(_worldPosition.x, _worldPosition.y, _worldPosition.z, 0.f));

	if (graphics->renderModelToShader(bone->getShader(), bone->getModel(), world, camera) == false)
		return false;

	return true;
}
