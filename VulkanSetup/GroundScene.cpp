#include "GroundScene.h"
#include "TransformStructure.h"
#include "BoneStructure.h"
#include "D3Device.h"
#include "Camera.h"
#include "Model.h"
#include "Graphics.h"
#include "Shader.h"
#include "MQuad.h"

GroundScene::GroundScene()
{
}

GroundScene::~GroundScene()
{
}

static int transformCount = 8;
void GroundScene::createScene(Graphics* graphics)
{
	_quad = graphics->createQuad(1.f, 100, 100);
	_quad->setRenderLine(true);
	_shader = graphics->createBaseShader();
}

void GroundScene::destroyScene()
{
	_shader->destroyShader();
	delete _shader;

	_quad->destroyModel();
	delete _quad;
}

void GroundScene::frame(float deltaTime)
{
	
}

bool GroundScene::renderScene(Graphics* graphics, Camera* camera)
{
	graphics->renderBegin(_quad);
	XMMATRIX worldMatrix = XMMatrixTranslationFromVector(_position);

	if (graphics->renderModelToShader(_shader, _quad, worldMatrix, camera) == false)
		return false;


	return true;
}
