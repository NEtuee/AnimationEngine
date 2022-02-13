#include <vector>
#include <functional>
#include <iostream>
#include <fstream>

#include "WindowApp.h"
#include "Graphics.h"

#include "ModelScene.h"
#include "LayerGhostModelScene.h"
#include "GUIScene.h"
#include "GroundScene.h"
#include "LayerGhostModelScene.h"
#include "Camera.h"

#include "Timer.h"
#include "Input.h"

#include "ResourceLoader.h"
#include "CharacterSet.h"
#include "CharacterSetLoader.h"

#include "BlendTree.h"
#include "AnimationLayerBase.h"
#include "AnimationSourceLayer.h"
#include "AnimationBlendLayer.h"

#include "HalfFloat.h"
#include "HalfQuaternion.h"
#include "Serialize.h"

#include "CompressedQuaternion.h"
#include "MathEx.h"
#include "Quantization.h"
#include "HalfFloat.h"

#include "ThreadPool.h"

#include "IKChain.h"
#include "LookAtIKSolver.h"
#include "IKSolver.h"
#include "BoneStructure.h"
#include "TransformStructure.h"

int main()
{
	Timer* timer = Timer::getInstance();
	Input* input = Input::getInstance();
	ThreadPool* threadPool = ThreadPool::getInstance();
	ResourceLoader* loader = ResourceLoader::getInstance();

	loader->createResourceLoader();
	timer->initialize();

	WindowApp	app(1280,720);
	Graphics	graphics;
	ModelScene	modelScene;
	GroundScene groundScene;
	GUIScene	guiScene;
	Camera		camera;

	camera.		createCamera();
	camera.		initialize();

	graphics.	initialize(app.getWidth(), app.getHeight(), app.getHWND());

	CharacterSetLoader csloader;
	CharacterSet* character = csloader.loadCharacterSet("Data/CharacterSet.las");
	character->	createCharacterSet("TestSet");

	modelScene.	setBoneStructure(character->getBoneStructure());
	modelScene.	setLayer(character->getBlendTree()->getLayer());

	modelScene.	createScene(&graphics);
	guiScene.	createScene(&graphics);
	groundScene.createScene(&graphics);

	guiScene.	setBoneStructure(modelScene.getBoneStructure());
	guiScene.	setCharacter(character);
	groundScene.setPosition(0.f,0.f,0.f);
	camera. 	setPosition(0.f,3.f,-5.f);

	std::vector<Scene*> scenes;
	scenes.		push_back(&modelScene);
	scenes.		push_back(&guiScene);
	scenes.		push_back(&groundScene);

	//for (float i = 0; i < 10; ++i)
	//{
	//	for (float j = 0; j < 10; ++j)
	//	{
	//		LayerGhostModelScene* ghost = new LayerGhostModelScene;
	//		ghost->setLayer(character->getBlendTree()->getLayer());
	//		ghost->setBoneStructure(character->getBoneStructure());
	//		ghost->setIKSolver(dynamic_cast<LookAtIKSolver*>(character->getIKChains()[0]->getSolver()));
	//		ghost->createScene(&graphics);

	//		ghost->setWorldPosition(XMFLOAT3(i + 1.f, 0.f, j + 1.f));
	//		//ghost->getBoneStructure()->find("root")->SetWorldPosition(XMFLOAT3(i + 1.f, 0.f, j + 1.f));
	//		scenes.push_back(ghost);
	//	}
	//}
	//

	bool reload = false;
	float deltaTime = 0.f;

	while (app.peekMessageCheck() != MessageState::M_QUIT)
	{
		MessageState currentState = app.getRecentlyState();
		if (currentState == MessageState::M_FALSE)
		{
			timer->		frame();
			deltaTime = timer->getDeltaTime();

			character->	frame(deltaTime);

			for (auto iterator = scenes.begin(); iterator != scenes.end(); ++iterator)
				(*iterator)->frame(deltaTime);

			camera.		frame(deltaTime);
			graphics.	frame();

			while (!threadPool->jobDone());

			graphics.	render(&scenes, &camera);
		}
		else if (currentState == MessageState::M_PEEK)
		{
			MSG msg = *app.getMessagePointer();
			if (msg.message == WM_KEYDOWN)
			{
				input->	setKeyDown(static_cast<unsigned int>(msg.wParam));
			}
			else if (msg.message == WM_KEYUP)
			{
				input->	setKeyUp(static_cast<unsigned int>(msg.wParam));
			}
		}

		if (input->isDown(VK_ESCAPE) && !reload)
		{
			character->		destroyCharacterSet();
			character = csloader.loadCharacterSet("Data/CharacterSet.las");
			character->		createCharacterSet("Test");

			modelScene.		destroyScene();
			modelScene.		setBoneStructure(character->getBoneStructure());
			modelScene.		setLayer(character->getBlendTree()->getLayer());
			modelScene.		createScene(&graphics);

			guiScene.		setBoneStructure(character->getBoneStructure());
			guiScene.		setCharacter(character);

			reload = true;
			
		}
		else if (input->isUp(VK_ESCAPE) && reload)
		{
			reload = false;
		}


		
	}

	for (auto iterator = scenes.begin(); iterator != scenes.end(); ++iterator)
	{
		(*iterator)->destroyScene();
	}

	camera.		destroyCamera();
	graphics.	destroyGraphics();
	app.		destroyWindowApp();

	character->	destroyCharacterSet();
	loader->	destroyResourceLoader();


	return 0;

	
}