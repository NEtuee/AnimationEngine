#pragma once
#include "GUIBase.h"
#include <Pdh.h>

class Triangulation;
class AnimationStateVariableFloat;
class AnimationStateBlendSpace;
class GUI_CurveEditorTest : public GUIBase
{
public:
	GUI_CurveEditorTest();
	~GUI_CurveEditorTest();

	void drawTriangles(ImDrawList* DrawList, ImRect rect);
	void setBlendSpace(AnimationStateBlendSpace* blendSpace) { _blendSpace = blendSpace; }
	void setTriangles(Triangulation* tri) { _tri = tri; }
	void setVariables(AnimationStateVariableFloat* row, AnimationStateVariableFloat* column);

	void updateVariables();

	bool IsOpen() { return _isOpen; }

	virtual void	initialize() override;
	virtual void	frame() override;
	virtual void	render() override;
private:
	ImVec2 toScreenCoord(ImVec2 world, ImRect rect);
	ImVec2 toWorldCoord(ImVec2 screen, ImRect rect);
private:
	ImVec2 _cursor;
	float _centerX;
	float _centerY;
	float _zoom;

	bool _isOpen;

	AnimationStateVariableFloat* _rowVar;
	AnimationStateVariableFloat* _columnVar;
	Triangulation* _tri;

	AnimationStateBlendSpace* _blendSpace;
};

