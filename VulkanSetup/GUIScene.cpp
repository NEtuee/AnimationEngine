#include "GUIScene.h"
#include "GUI_FPSAndCPUCounter.h"
#include "GUI_BoneStructureViewer.h"
#include "GUI_BlendTreeViewer.h"
#include "GUI_CurveEditorTest.h"
#include "CharacterSet.h"

GUIScene::GUIScene()
	:_guiCounter(nullptr),_guiBoneView(nullptr), _guiTreeView(nullptr)
{
}

GUIScene::~GUIScene()
{
}

void GUIScene::setBoneStructure(BoneStructure* target)
{
	_guiBoneView->set(target);
}

void GUIScene::setCharacter(CharacterSet* set)
{
	_guiTreeView->set(set);
}

void GUIScene::createScene(Graphics* graphics)
{
	_guiCounter = new GUI_FPSAndCPUCounter;
	_guiCounter->initialize();

	_guiBoneView = new GUI_BoneStructureViewer;
	_guiBoneView->initialize();

	_guiTreeView = new GUI_BlendTreeViewer;
	_guiTreeView->initialize();


}

void GUIScene::destroyScene()
{
	delete _guiCounter;
	delete _guiBoneView;
	delete _guiTreeView;

}

void GUIScene::frame(float deltaTime)
{
	_guiCounter->frame();
	_guiBoneView->frame();
	_guiTreeView->frame();

}

bool GUIScene::renderScene(Graphics* dc, Camera* camera)
{
	_guiCounter->render();
	_guiBoneView->render();
	_guiTreeView->render();


	return true;
}
