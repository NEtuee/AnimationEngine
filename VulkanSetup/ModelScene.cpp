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

void ModelScene::createGhosts(Graphics* graphics, std::vector<Scene*>& container, AnimationLayerBase* layer)
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
		//ghost->createScene()
	}
}

void ModelScene::setBoneStructure(BoneStructure* bone)
{
	_boneStructure = bone;
}

void ModelScene::createScene(Graphics* graphics)
{
	clock_t start, end;
	double result;
	start = clock();

	//for (const auto& file : std::filesystem::directory_iterator("Resource/"))
	//{
	//	if (file.path().extension().compare(".dat") == 0)
	//	{
	//		ResourceLoader::getInstance()->preloadAnimation(file.path().string());
	//	}
	//}


	//while (!ResourceLoader::getInstance()->jobDone());


	end = clock() - start;
	result = static_cast<double>(end);
	std::cout << "animation load all : " << result << "ms" << std::endl;

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

	bone->destroyBoneStructure();
	delete bone;
}

void ModelScene::frame(float deltaTime)
{

}

bool ModelScene::renderScene(Graphics* graphics, Camera* camera)
{
	auto bone = _boneStructure;
	graphics->renderBegin(bone->getModel());

	renderBoneStructure(&bone->getRootBone(), graphics, camera);


	graphics->renderBegin(_weaponModel);
	if (graphics->renderModelToShader(bone->getShader(), _weaponModel, _realSword->getMatrix(), camera) == false)
	{
		return false;
	}
	if (graphics->renderModelToShader(bone->getShader(), _weaponModel, _shield->getMatrix(), camera) == false)
	{
		return false;
	}

	return true;
}

bool ModelScene::renderBoneStructure(TransformStructure* root, Graphics* graphics, Camera* camera)
{
	if (root->isCanDraw() == false)
		return true;

	auto children = root->getChildren();
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
