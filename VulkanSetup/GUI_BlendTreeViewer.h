#pragma once
#include "GUIBase.h"
#include <unordered_map>

class BoneStructure;
class AnimationLayerBase;
class TransformStructure;
class GUI_CurveEditorTest;
class CharacterSet;
class GUI_BlendTreeViewer : public GUIBase
{
public:
	GUI_BlendTreeViewer();
	GUI_BlendTreeViewer(const GUI_BlendTreeViewer&);
	~GUI_BlendTreeViewer();

	void			set(CharacterSet* target);

	virtual void	initialize() override;
	virtual void	frame() override;
	virtual void	render() override;
private:
	void			renderStructure(AnimationLayerBase* layer, float depth);
private:
	CharacterSet* _character;
	AnimationLayerBase* _targetLayer;
	GUI_CurveEditorTest* _curveEditor;
	std::unordered_map<AnimationLayerBase*, bool> _openMap;

	bool _curveEditorOpen;
};
