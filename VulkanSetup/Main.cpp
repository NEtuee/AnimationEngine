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

int main()
{ 
	WindowApp	app(1280,720);
	Graphics	graphics;
	ModelScene	modelScene;
	GroundScene groundScene;
	GUIScene	guiScene;
	Camera		camera;

	CharacterSetLoader csloader;
	CharacterSet* character = csloader.loadCharacterSet("Data/CharacterSet.las");
	character->createCharacterSet("TestSet");

	Timer* timer =				Timer::getInstance();
	Input* input =				Input::getInstance();
	ResourceLoader* loader =	ResourceLoader::getInstance();

	loader->createResourceLoader();
	timer->initialize();
	graphics.initialize(app.getWidth(), app.getHeight(), app.getHWND());

	modelScene.setBoneStructure(character->getBoneStructure());
	modelScene.createScene(&graphics);
	
	guiScene.createScene(&graphics);
	groundScene.createScene(&graphics);

	camera.createCamera();
	camera.initialize();

	guiScene.setBoneStructure(modelScene.getBoneStructure());
	guiScene.setCharacter(character);
	groundScene.setPosition(0.f,0.f,0.f);
	camera.setPosition(0.f,3.f,-5.f);

	std::vector<Scene*> scenes;
	scenes.push_back(&modelScene);
	scenes.push_back(&guiScene);
	scenes.push_back(&groundScene);

	

	bool reload = false;

	while (app.peekMessageCheck() != MessageState::M_QUIT)
	{
		if (app.getRecentlyState() == MessageState::M_FALSE)
		{
			timer->frame();
			float deltaTime = timer->getTime();

			character->frame(deltaTime);

			for (auto iterator = scenes.begin(); iterator != scenes.end(); ++iterator)
			{
				(*iterator)->frame(deltaTime);
			}

			camera.frame(deltaTime);
			graphics.frame();


			graphics.render(&scenes, &camera);
		}
		else if (app.getRecentlyState() == MessageState::M_PEEK)
		{
			MSG msg = *app.getMessagePointer();
			if (msg.message == WM_KEYDOWN)
			{
				input->setKeyDown(static_cast<unsigned int>(msg.wParam));
			}
			else if (msg.message == WM_KEYUP)
			{
				input->setKeyUp(static_cast<unsigned int>(msg.wParam));
			}
		}

		if (input->isDown(VK_ESCAPE))
		{
			modelScene.destroyScene();
			modelScene.createScene(&graphics);
			guiScene.setBoneStructure(modelScene.getBoneStructure());
			guiScene.setCharacter(character);
			
		}
	}

	modelScene.destroyScene();
	guiScene.destroyScene();
	groundScene.destroyScene();
	camera.destroyCamera();


	graphics.destroyGraphics();
	app.destroyWindowApp();
	loader->destroyResourceLoader();


	return 0;

	
}