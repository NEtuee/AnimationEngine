#include "GUI_BlendTreeViewer.h"
#include "BoneStructure.h"
#include "TransformStructure.h"
#include "MathEx.h"
#include "AnimationLayerBase.h"
#include "AnimationBlendLayer.h"
#include "AnimationSourceLayer.h"
#include "Model.h"
#include "CharacterSet.h"
#include "BlendTree.h"
#include "AnimationStateVariable.h"
#include "AnimationStateBlendSpace.h"
#include "AnimationStateBase.h"
#include "GUI_CurveEditorTest.h"
#include "Shader.h"

GUI_BlendTreeViewer::GUI_BlendTreeViewer()
{
}

GUI_BlendTreeViewer::GUI_BlendTreeViewer(const GUI_BlendTreeViewer&)
{
}

GUI_BlendTreeViewer::~GUI_BlendTreeViewer()
{
	delete _curveEditor;
}

void GUI_BlendTreeViewer::set(CharacterSet* target)
{
	_character = target;
	_targetLayer = target->getBlendTree()->getLayer();

	_curveEditorOpen = false;

}

void GUI_BlendTreeViewer::initialize()
{
	_curveEditor = new GUI_CurveEditorTest;
	_curveEditorOpen = false;
}

void GUI_BlendTreeViewer::frame()
{
}

void GUI_BlendTreeViewer::render()
{
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Blend Tree Viewer");

	renderStructure(_targetLayer, 10.f);

	ImGui::Spacing();

	ImGui::Text("variables");

	auto& variables = _character->getBlendTree()->getVariables();
	for (auto iter = variables.begin(); iter != variables.end(); ++iter)
	{
		StateVariableTypeEnum type = (*iter).second->getType();

		if (type == StateVariableTypeEnum::Bool)
		{
			bool var = *static_cast<bool*>(iter->second->getVariable());
			ImGui::Checkbox(iter->second->getName().c_str(), &var);
			iter->second->setVariable(&var);
		}
		else if (type == StateVariableTypeEnum::Float)
		{
			float var = *static_cast<float*>(iter->second->getVariable());
			ImGui::SliderFloat(iter->second->getName().c_str(), &var,-10.f,10.f);
			iter->second->setVariable(&var);
		}
	}

	ImGui::End();

	if (_curveEditorOpen)
	{
		_curveEditor->render();
		_curveEditorOpen = _curveEditor->IsOpen();
	}
}

static const char* openLabel[] = { "x##1","o##1" };
void GUI_BlendTreeViewer::renderStructure(AnimationLayerBase* layer, float depth)
{
	if (_openMap.find(layer) == _openMap.end())
		_openMap.insert(std::make_pair(layer, true));

	bool isOpen = _openMap[layer];

	ImGui::Spacing();
	ImGui::SameLine(depth);

	std::string label = openLabel[isOpen] + layer->getName();

	if (ImGui::Button(label.c_str()))
	{
		_openMap[layer] = !_openMap[layer];
	}

	ImGui::SameLine();
	ImGui::Text(layer->getName().c_str()); //ImGui::SameLine();

	AnimationBlendLayer* isBlendlayer = dynamic_cast<AnimationBlendLayer*>(layer);

	ImGui::SameLine();
	ImGui::Text(isBlendlayer ? ": blend Layer" : ": source Layer");

	if (_openMap[layer] == false)
		return;
	if (isBlendlayer)
	{
		if (isBlendlayer->getMaskBone())
		{
			ImGui::SameLine();
			ImGui::Text("partial");
		}

		if (isBlendlayer->IsAdditive())
		{
			ImGui::SameLine();
			ImGui::Text("additive");
		}


		ImGui::Spacing();
		ImGui::SameLine(depth + 15.f);
		std::string key = "##" + isBlendlayer->getName();
		float weight = isBlendlayer->getWeight();
		ImGui::SliderFloat(key.c_str(), &weight, 0.f, 1.f);
		isBlendlayer->setWeight(weight);

		

		renderStructure(isBlendlayer->getLayerOne(), depth + 15.f);
		renderStructure(isBlendlayer->getLayerTwo(), depth + 15.f);
	}
	else
	{
		AnimationSourceLayer* source = dynamic_cast<AnimationSourceLayer*>(layer);
		ImGui::Spacing(); ImGui::SameLine(depth + 15.f);
		ImGui::Text("states :");

		ImGui::SameLine();
		ImGui::Text(source->getCurrentState() == nullptr ? "null" : source->getCurrentState()->getName().c_str());
		auto& states = source->getStates();
		for (auto iter = states.begin(); iter != states.end(); ++iter)
		{
			ImGui::Spacing(); ImGui::SameLine(depth + 35.f);
			ImGui::Text((*iter)->getName().c_str());

			auto blendState = dynamic_cast<AnimationStateBlendSpace*>((*iter));
			if (blendState != nullptr)
			{
				ImGui::SameLine();
				std::string blendSpaceKey = "open##bs" + (*iter)->getName();
				if (ImGui::Button(blendSpaceKey.c_str()))
				{
					_curveEditor->setTriangles(blendState->getTriangle());
					_curveEditor->setVariables(blendState->getRowVariable(), blendState->getColumnVariable());
					_curveEditor->setBlendSpace(blendState);
					_curveEditor->initialize();

					_curveEditorOpen = true;
				}
			}
		}
	}
}
