#include "GUI_BoneStructureViewer.h"
#include "BoneStructure.h"
#include "TransformStructure.h"
#include "MathEx.h"
#include "Model.h"
#include "Shader.h"

static XMVECTOR rotationCompare{ 0 };

GUI_BoneStructureViewer::GUI_BoneStructureViewer()
{
}

GUI_BoneStructureViewer::GUI_BoneStructureViewer(const GUI_BoneStructureViewer&)
{
}

GUI_BoneStructureViewer::~GUI_BoneStructureViewer()
{

}

void GUI_BoneStructureViewer::set(BoneStructure* target)
{
	_targetBoneStructure = target;
	select(target->getStructureTransform());
}

void GUI_BoneStructureViewer::select(TransformStructure* target)
{
	_selectedTransform = target;
	updateToLocal();
}

void GUI_BoneStructureViewer::initialize()
{
	
}

void GUI_BoneStructureViewer::frame()
{
	//if (isValueChagned());
	updateToLocal();
}

void GUI_BoneStructureViewer::render()
{
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Bone Transform");

	ImGui::Text("name : "); ImGui::SameLine(); ImGui::Text(_selectedTransform->getName().c_str());
	
	if (ImGui::Button("Local", ImVec2(ImGui::GetWindowSize().x * 0.5f - 10.f, 0.f)))
		_showWorld = false;
	ImGui::SameLine();
	if (ImGui::Button("World", ImVec2(ImGui::GetWindowSize().x * 0.5f - 10.f, 0.f)))
		_showWorld = true;

	if (_showWorld)
	{
		XMFLOAT3 pos, scale;
		XMFLOAT4 rotation;
		XMStoreFloat3(&pos, _selectedTransform->getWorldPosition());
		XMStoreFloat4(&rotation, _selectedTransform->getLocalRotation());
		XMStoreFloat3(&scale, _selectedTransform->getWorldScale());

		//rotation = MathEx::radianToDegree(MathEx::quaternionToEuler(_selectedTransform->getWorldRotation()));

		ImGui::Text("pos  : "); ImGui::SameLine(); ImGui::Text("%.3f %.3f %.3f", pos.x, pos.y, pos.z);
		ImGui::Text("rot  : "); ImGui::SameLine(); ImGui::Text("%.3f %.3f %.3f %.3f", rotation.x, rotation.y, rotation.z, rotation.w);
		ImGui::Text("scl  : "); ImGui::SameLine(); ImGui::Text("%.3f %.3f %.3f", scale.x, scale.y, scale.z);
	}
	else
	{
		ImGui::Text("pos  : "); ImGui::SameLine(); ImGui::SliderFloat3("##1", _positionInput,-2.f,2.f);
		ImGui::Text("rot  : "); ImGui::SameLine(); ImGui::SliderFloat4("##2", _rotationInput,0.f,3.141592f);
		ImGui::Text("scl  : "); ImGui::SameLine(); ImGui::SliderFloat3("##3", _scaleInput,0.1f,5.f);
	}
	
	ImGui::End();

	if (ImGui::Begin("Bone List") == true)
	{
		renderStructure(_targetBoneStructure->getStructureTransform(), 10.f);
	}
	ImGui::End();

	updateToTarget();
}

void GUI_BoneStructureViewer::updateToLocal()
{
	XMFLOAT3 pos, scale;
	XMFLOAT4 rotation;
	XMStoreFloat3(&pos, _selectedTransform->getLocalPosition());
	XMStoreFloat4(&rotation, _selectedTransform->getLocalRotation());
	XMStoreFloat3(&scale, _selectedTransform->getLocalScale());

	_positionInput[0] = pos.x;
	_positionInput[1] = pos.y;
	_positionInput[2] = pos.z;

	//rotation = MathEx::radianToDegree(MathEx::quaternionToEuler(_selectedTransform->getLocalRotation()));

	if (XMQuaternionEqual(rotationCompare, _selectedTransform->getLocalRotation()) == false)
	{
		_rotationInput[0] = (rotation.x);
		_rotationInput[1] = (rotation.y);
		_rotationInput[2] = (rotation.z);
		_rotationInput[3] = (rotation.w);

		rotationCompare = _selectedTransform->getLocalRotation();
	}
	

	_scaleInput[0] = scale.x;
	_scaleInput[1] = scale.y;
	_scaleInput[2] = scale.z;
}

void GUI_BoneStructureViewer::updateToTarget()
{
	XMFLOAT3 pos{ _positionInput[0],_positionInput[1] ,_positionInput[2] };
	_selectedTransform->SetLocalPosition(pos);

	////XMFLOAT3 rotation{ XMConvertToRadians(_rotationInput[0]),XMConvertToRadians(_rotationInput[1]) ,XMConvertToRadians(_rotationInput[2]) };
	//XMVECTOR rotation = XMVectorSet(_rotationInput[0], _rotationInput[1], _rotationInput[2], _rotationInput[3]);
	//_selectedTransform->SetLocalRotation(rotation);
	//rotationCompare = rotation;// XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	//XMFLOAT3 scale{ _scaleInput[0],_scaleInput[1] ,_scaleInput[2] };
	//_selectedTransform->SetLocalScale(scale);
}

const char* openLabel[] = { "x##1","o##1" };
void GUI_BoneStructureViewer::renderStructure(TransformStructure* root, float depth)
{
	if (_openMap.find(root) == _openMap.end())
		_openMap.insert(std::make_pair(root, root->isCanDraw()));

	bool isOpen = _openMap[root];

	ImGui::SameLine(depth);
	bool selected = _selectedTransform == root;
	
	std::string label = openLabel[isOpen] + root->getName();
	
	if (ImGui::Button(label.c_str()))
	{
		_openMap[root] = !_openMap[root];
	}

	ImGui::SameLine();

	bool canDraw = root->isCanDraw();
	label = "##" + root->getName();
	ImGui::Checkbox(label.c_str(), &canDraw);
	root->canDraw(canDraw);

	ImGui::SameLine();

	if(selected)
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0,255,0,255));

	if (ImGui::Button(root->getName().c_str(), ImVec2(130.f, 0.f)))
	{
		select(root);
	}

	if (selected)
		ImGui::PopStyleColor();

	ImGui::SameLine();
	ImGui::Text("%d", root->getChildren().size());

	ImGui::NewLine();

	if (_openMap[root] == false)
		return;

	auto children = root->getChildren();
	for (auto iterator = children.begin(); iterator != children.end(); ++iterator)
	{
		renderStructure((*iterator), depth + 10.f);
	}
}
