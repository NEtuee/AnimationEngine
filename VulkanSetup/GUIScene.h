#pragma once
#include "Scene.h"

class BoneStructure;
class GUI_FPSAndCPUCounter;
class GUI_BoneStructureViewer;
class GUI_BlendTreeViewer;
class CharacterSet;
class GUIScene : public Scene
{
public:
								GUIScene();
								~GUIScene();

	void						setBoneStructure(BoneStructure* target);
	void						setCharacter(CharacterSet* layer);

	virtual void				createScene(Graphics*) override;
	virtual void				destroyScene() override;
	virtual void				frame(float) override;
	virtual bool				renderScene(Graphics*, Camera*) override;
private:
	GUI_FPSAndCPUCounter*		_guiCounter;
	GUI_BoneStructureViewer*	_guiBoneView;
	GUI_BlendTreeViewer*		_guiTreeView;
};

