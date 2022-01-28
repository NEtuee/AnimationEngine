#include "GUI_CurveEditorTest.h"
#include "Timer.h"
#include "Triangulation.h"
#include "AnimationStateVariableFloat.h"
#include "AnimationStateBlendSpace.h"
#include "AnimationDataPack.h"
#include <vector>
#include <windows.h>

GUI_CurveEditorTest::GUI_CurveEditorTest()
	:_centerX(0.f), _centerY(0.f), _zoom(1.f), _tri(nullptr), _cursor(0,0)
{
}


GUI_CurveEditorTest::~GUI_CurveEditorTest()
{

}

void GUI_CurveEditorTest::drawTriangles(ImDrawList* DrawList, ImRect rect)
{
	auto& points = _tri->getPoints();
	auto& triangles = _tri->getTriangles();
	auto& items = _blendSpace->getItems();

	for (int i = 0; i < triangles.size(); ++i)
	{
		auto point = points[triangles[i]._a];
		auto point1 = toScreenCoord(ImVec2(point.x, -point.y), rect);

		point = points[triangles[i]._b];
		auto point2 = toScreenCoord(ImVec2(point.x, -point.y), rect);

		point = points[triangles[i]._c];
		auto point3 = toScreenCoord(ImVec2(point.x, -point.y), rect);

		DrawList->AddLine(point1, point2,ImGui::GetColorU32(ImGuiCol_TextDisabled));
		DrawList->AddLine(point2, point3, ImGui::GetColorU32(ImGuiCol_TextDisabled));
		DrawList->AddLine(point3, point1, ImGui::GetColorU32(ImGuiCol_TextDisabled));
		
		ImGui::SetCursorScreenPos(point1 - ImVec2(6, 6));
		std::string key = "##" + i + items[triangles[i]._a]._animation->getName();
		ImGui::Button(key.c_str(), ImVec2(2 * 6, 2 * 6));
		if (ImGui::IsItemActive() || ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(items[triangles[i]._a]._animation->getName().c_str());
		}


		ImGui::SetCursorScreenPos(point2 - ImVec2(6, 6));
		key = "##" + i + items[triangles[i]._b]._animation->getName();
		ImGui::Button(key.c_str(), ImVec2(2 * 6, 2 * 6));
		if (ImGui::IsItemActive() || ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(items[triangles[i]._b]._animation->getName().c_str());
		}


		ImGui::SetCursorScreenPos(point3 - ImVec2(6, 6));
		key = "##" + i + items[triangles[i]._c]._animation->getName();
		ImGui::Button(key.c_str(), ImVec2(2 * 6, 2 * 6));
		if (ImGui::IsItemActive() || ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(items[triangles[i]._c]._animation->getName().c_str());
		}
	}

	if (_blendSpace->isPointOut())
	{
		XMFLOAT3 nearPoint;
		XMStoreFloat3(&nearPoint, _blendSpace->getNearestPoint());
		DrawList->AddLine(toScreenCoord(_cursor * 100.f,rect), toScreenCoord(ImVec2(nearPoint.x, -nearPoint.y),rect), ImGui::GetColorU32(ImGuiCol_TextDisabled));
	}
}

void GUI_CurveEditorTest::setVariables(AnimationStateVariableFloat* row, AnimationStateVariableFloat* column)
{
	_rowVar = row;
	_columnVar = column;

	_cursor.x = *static_cast<float*>(_rowVar->getVariable()) * 0.01f;
	_cursor.y = -(*static_cast<float*>(_columnVar->getVariable())) * 0.01f;
}

void GUI_CurveEditorTest::updateVariables()
{
	float row = _cursor.x * 100.f;
	float column = -_cursor.y * 100.f;
	_rowVar->setVariable(&row);
	_columnVar->setVariable(&column);
}

void GUI_CurveEditorTest::initialize()
{
	_centerX = 0.f;
	_centerY = 0.f;
	_zoom = 1.f;
	_isOpen = true;
}

void GUI_CurveEditorTest::frame()
{

}

void GUI_CurveEditorTest::render()
{
	bool changed = false;


	ImGui::SliderFloat("zoom", &_zoom, 0.1f, 10.f);
	ImGui::SameLine();
	if (ImGui::Button("Close"))
	{
		_isOpen = false;
		return;
	}

	ImDrawList* DrawList = ImGui::GetWindowDrawList();
	ImGuiWindow* Window = ImGui::GetCurrentWindow();
	if (Window->SkipItems)
		return;

	// header and spacing
	ImGui::Text("Blend Space");
	int hovered = ImGui::IsItemActive() || ImGui::IsItemHovered(); // IsItemDragged() ?
	ImGui::Dummy(ImVec2(0, 3));

	// prepare canvas
	const float avail = ImGui::GetContentRegionAvailWidth();
	const float dim = ImMin(avail, 300.f);
	ImVec2 Canvas(dim, dim);

	ImRect bb(Window->DC.CursorPos, Window->DC.CursorPos + Canvas);
	ImGui::ItemSize(bb);
	if (!ImGui::ItemAdd(bb, NULL))
		return;

	const ImGuiID id = Window->GetID("Check");
	hovered |= 0 != ImGui::IsHovered(ImRect(bb.Min, bb.Min + ImVec2(avail, dim)), id);

	ImGui::RenderFrame(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg, 1), true);

	// background grid
	//for (int i = 0; i <= Canvas.x; i += (Canvas.x / 4)) {
	//	DrawList->AddLine(
	//		ImVec2(bb.Min.x + i, bb.Min.y),
	//		ImVec2(bb.Min.x + i, bb.Max.y),
	//		ImGui::GetColorU32(ImGuiCol_TextDisabled));
	//}
	//for (int i = 0; i <= Canvas.y; i += (Canvas.y / 4)) {
	//	DrawList->AddLine(
	//		ImVec2(bb.Min.x, bb.Min.y + i),
	//		ImVec2(bb.Max.x, bb.Min.y + i),
	//		ImGui::GetColorU32(ImGuiCol_TextDisabled));
	//}


	ImVec2 pos = _cursor * (bb.Max - bb.Min) + bb.Min;
	ImGui::SetCursorScreenPos((((_cursor * _zoom) * (bb.Max - bb.Min) + bb.Min) + ImVec2(150.f + _centerX, 150.f + _centerY)) - ImVec2(6, 6));
	ImGui::InvisibleButton("##button", ImVec2(2 * 6, 2 * 6));
	if (ImGui::IsItemActive() || ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("%f %f", _cursor.x * 100.f, -_cursor.y * 100.f);
	}
	if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
	{
		_cursor.x += ImGui::GetIO().MouseDelta.x / Canvas.x / _zoom;
		_cursor.y += ImGui::GetIO().MouseDelta.y / Canvas.y / _zoom;
		
		changed = true;
	}
	else if (ImGui::IsMouseDragging(1))
	{
		_centerX += ImGui::GetIO().MouseDelta.x;
		_centerY += ImGui::GetIO().MouseDelta.y;
	}

	if (ImGui::GetIO().MouseWheel > 0)
	{
		_zoom += 0.1f;
	}
	if (ImGui::GetIO().MouseWheel < 0)
	{
		_zoom -= 0.1f;
		_zoom = _zoom < 0.5f ? 0.5f : _zoom;
	}

	if (hovered || changed)
	{
		DrawList->PushClipRectFullScreen();

		updateVariables();
	}

	drawTriangles(DrawList, bb);

	ImVec4 white(ImGui::GetStyle().Colors[ImGuiCol_Text]);
	ImVec2 p1 = (((ImVec2(_cursor.x, _cursor.y) * _zoom) * (bb.Max - bb.Min) + bb.Min) + ImVec2(150.f + _centerX,150.f + _centerY));

	DrawList->AddCircleFilled(p1, 6, ImColor(white));


	if (hovered || changed) DrawList->PopClipRect();
}

ImVec2 GUI_CurveEditorTest::toScreenCoord(ImVec2 world, ImRect rect)
{
	return (((world * 0.01f) * _zoom) * (rect.Max - rect.Min) + rect.Min) + ImVec2(150.f + _centerX, 150.f + _centerY);
}

ImVec2 GUI_CurveEditorTest::toWorldCoord(ImVec2 screen, ImRect rect)
{
	//screen - ImVec2(150.f + _centerX, 150.f + _centerY)
	return ImVec2();
}
