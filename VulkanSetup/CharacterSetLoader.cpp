#include "CharacterSetLoader.h"
#include "XMLLoader.h"
#include "XMLData.h"
#include "ResourceLoader.h"
#include "BoneStructure.h"
#include "CharacterSet.h"
#include "AnimationDataPack.h"
#include "AnimationStateVariable.h"
#include "AnimationStateVariableFloat.h"
#include "AnimationStateLayer.h"
#include "AnimationStateBase.h"
#include "AnimationStateSingle.h"
#include "AnimationStateBlendSpace.h"
#include "AnimationStateCondition.h"
#include "AnimationStateConnection.h"
#include "AnimationLayerBase.h"
#include "AnimationBlendLayer.h"
#include "AnimationSourceLayer.h"
#include "BlendTree.h"
#include "StringEx.h"
#include <iostream>
#include <cassert>

CharacterSet* CharacterSetLoader::loadCharacterSet(std::string path)
{
    XMLLoader loader;
    auto xml = loader.loadXML_c(path);

    std::string skeletonPath = xml->findElement("Skeleton")->findData("FilePath")->data;

    CharacterSet* character = new CharacterSet;
    character->_bone = ResourceLoader::getInstance()->getSkeleton(skeletonPath);

    BlendTree* blendTree = new BlendTree;
    blendTree->createBlendTree(character->_bone);
	character->_tree = blendTree;

    auto animationElem = xml->findElement("Animations");
    for (int i = 0; i < animationElem->_childs.size(); ++i)
    {
        auto info = animationElem->_childs[i];
        std::string name = info->getProperty("Name");
        std::string path = info->getProperty("FilePath");
        bool isLoop = info->getProperty("IsLoop") == "True";
        float speed = std::stof(info->getProperty("Speed"));

        auto pack = ResourceLoader::getInstance()->getAnimation(path);
        pack->setLoop(isLoop);
        pack->setSpeed(speed);

        character->_animations.emplace(name, pack);
    }

    auto bt = xml->findElement("BlendTree");
    auto variableElem = bt->findElement("Variables");
    for (int i = 0; i < variableElem->_childs.size(); ++i)
    {
        auto info = variableElem->_childs[i];
        std::string name = info->getProperty("Name");
        StateVariableTypeEnum type = stringToTypeEnum(info->getProperty("Type"));

        blendTree->createVariable(type, name, nullptr)
            ->setStringToVariable(info->getProperty("DefaultValue"));
    }

    auto layerElem = bt->findElement("Layers");
	std::unordered_map<std::string, AnimationLayerBase*> layers;
	std::unordered_map<std::string, std::pair<std::string,std::string>> blendLayerTargets;
    for (int i = 0; i < layerElem->_childs.size(); ++i)
    {
        auto info = layerElem->_childs[i];
		AnimationLayerBase* layer = nullptr;

		if (info->name.compare("BlendLayer") == 0)
		{
			layer = createBlendLayer(character, blendTree, info, skeletonPath);
			blendLayerTargets.emplace(layer->getName(), std::make_pair(info->getProperty("Layer1"), info->getProperty("Layer2")));
		}
		else if (info->name.compare("SourceLayer") == 0)
		{
			layer = createSourceLayer(character, blendTree, info);
		}

		layers.emplace(layer->getName(), layer);
    }

	for (auto iterator = blendLayerTargets.begin(); iterator != blendLayerTargets.end(); ++iterator)
	{
		auto infoSet = (*iterator);
		if (infoSet.first.compare(infoSet.second.first) == 0 ||
			infoSet.first.compare(infoSet.second.second) == 0)
		{
			assert(false);
			return nullptr;
		}

		auto layerOne = layers.find(infoSet.second.first);
		auto layerTwo = layers.find(infoSet.second.second);

		if (layerOne == layers.end() || layerTwo == layers.end() || 
			(*layerOne).first.compare((*layerTwo).first) == 0)
		{
			assert(false);
			return nullptr;
		}

		dynamic_cast<AnimationBlendLayer*>(layers[iterator->first])->setLayers((*layerOne).second, (*layerTwo).second);
	}

	auto resultFind = layers.find(layerElem->getProperty("ResultLayer"));
	if (resultFind == layers.end())
	{
		assert(false);
		return nullptr;
	}

	auto resultLayer = (*resultFind).second;
	resultLayer->initialize();

	blendTree->setResultLayer(resultLayer);

	layers.clear();
	blendLayerTargets.clear();
    
    xml->destroyChildren();
    delete xml;

    return character;
}

AnimationLayerBase* CharacterSetLoader::createSourceLayer(CharacterSet* character, BlendTree* blendTree, XMLElement* element)
{
	std::string name = element->getProperty("Name");
	std::string defaultState = element->getProperty("DefaultState");

    auto layer = new AnimationSourceLayer;
    layer->createAnimationLayerBase(name);

	auto nodeElem = element->findElement("Nodes");
	for (int j = 0; j < nodeElem->_childs.size(); ++j)
	{
		auto nodeInfo = nodeElem->_childs[j];

		if (nodeInfo->name.compare("StateNode") == 0)
		{
			std::string name = nodeInfo->_property.begin()->second;
			AnimationStateSingle* state = new AnimationStateSingle;

			auto animationElem = nodeInfo->findElement("Animation");
			if (animationElem != nullptr)
			{
				state->createAnimationState(name);
				state->setAnimation(character->_animations[animationElem->_property.begin()->second]);
			}

			layer->addState(state);
		}
		else if (nodeInfo->name.compare("BlendSpace") == 0)
		{
			std::string name = nodeInfo->_property.begin()->second;
			AnimationStateBlendSpace* state = new AnimationStateBlendSpace;
			state->createAnimationState(name);

			auto& children = nodeInfo->_childs;
			for (int i = 0; i < children.size(); ++i)
			{
				std::string aniName = children[i]->getProperty("Name");
				float row = std::stof(children[i]->getProperty("Row"));
				float column = std::stof(children[i]->getProperty("Column"));

				state->addAnimationkey(character->_animations[aniName], row, column);
			}

			std::string rowVar = nodeInfo->getProperty("RowVariable");
			std::string colVar = nodeInfo->getProperty("ColumnVariable");

			state->createBlendSpace(dynamic_cast<AnimationStateVariableFloat*>(blendTree->getVariable(rowVar)),
				dynamic_cast<AnimationStateVariableFloat*>(blendTree->getVariable(colVar)));

			layer->addState(state);
		}
		
	}

	if(defaultState != "")
		layer->setDefaultState(defaultState);
	//layer->initialize();

	auto connectionElem = element->findElement("Connections");
	if (connectionElem == nullptr)
	{
		return layer;
	}

	for (int j = 0; j < connectionElem->_childs.size(); ++j)
	{
		auto connectionInfo = connectionElem->_childs[j];
		std::string fromNode = connectionInfo->getProperty("From");
		std::string conditionString = connectionInfo->getProperty("Condition");
		std::string toNode = connectionInfo->getProperty("To");
		float blendTime = std::stof(connectionInfo->getProperty("BlendTime"));

		auto condition = stringToCondition(conditionString, blendTree);

		AnimationStateConnection* connection = new AnimationStateConnection;
		connection->createAnimationStateConnection(layer->findState(toNode), blendTime);
		if(condition != nullptr)
			connection->addCondition(condition);

		layer->findState(fromNode)->addConnection(connection);
	}
    return layer;
}

AnimationLayerBase* CharacterSetLoader::createBlendLayer(CharacterSet* character, BlendTree* blendTree, XMLElement* element, std::string skeletonPath)
{
	auto additive = element->findElement("Additive");
	auto partialMask = element->findElement("PartialMask");

	std::string name = element->getProperty("Name");
	float defaultWeight = std::stof(element->getProperty("DefaultWeight"));


	auto layer = new AnimationBlendLayer;
	layer->createAnimationLayerBase(name);
	layer->setWeight(defaultWeight);

	if (additive != nullptr)
	{
		std::string basePose = additive->getProperty("BasePose");
		bool isAdditive = additive->getProperty("IsAdditive").compare("True") == 0;

		layer->setAdditive(isAdditive);
		layer->setAdditiveBasePose(basePose == "" ?
			ResourceLoader::getInstance()->getSkeletonPose(skeletonPath) :
			character->_animations[basePose]);
	}


	if (partialMask != nullptr)
	{
		std::string maskBone = partialMask->getProperty("MaskBone");
		//std::string inverse = partialMask->getProperty("Inverse");
		std::string rotationRoot = partialMask->getProperty("RotationRoot");
		float maskDepth = std::stof(partialMask->getProperty("MaskDepth"));
		float smoothCount = std::stof(partialMask->getProperty("SmoothCount"));

		layer->setMaskBone((maskBone == "" ? nullptr : character->_bone->find(maskBone)));
		layer->setRotationRootBone(rotationRoot == "" ? nullptr : character->_bone->find(rotationRoot));
		layer->setCharacterRoot(&character->_bone->getRootBone());
		layer->setMaskDepth(maskDepth);
		layer->setSmoothCount(smoothCount);
		
		//layer->setInversMasking(inverse == "True");
	}

    return layer;
}

StateVariableTypeEnum CharacterSetLoader::stringToTypeEnum(std::string str)
{
    if (str.compare("Bool") == 0)
        return StateVariableTypeEnum::Bool;
    else if (str.compare("Float") == 0)
        return StateVariableTypeEnum::Float;
    else if (str.compare("Int") == 0)
        return StateVariableTypeEnum::Int;

    return StateVariableTypeEnum();
}

AnimationStateCondition* CharacterSetLoader::stringToCondition(std::string str, BlendTree* blendTree)
{
	if (str == "")
		return nullptr;

    str = StringEx::replaceAll(str, " ", "");

    std::string sign = "";
    auto conditionEnum = findConditionInString(str, sign);
    
    str = str.replace(str.find(sign),sign.size(), " ");

    std::vector<std::string> split;
    StringEx::split(split, str, ' ');

    auto variable = blendTree->getVariable(split[0]);
    char* stv = variable->stringToVariable(split[1]);

    AnimationStateCondition* condition = new AnimationStateCondition;
    condition->createAnimationStateCondition(variable, conditionEnum);
    condition->setTarget(*stv);

    delete stv;

    return condition;
}

StateConditions CharacterSetLoader::findConditionInString(std::string str, std::string& sign)
{
    if (str.find("<=") != std::string::npos)
    {
        sign = "<=";
        return StateConditions::SmallerEqual;
    }
	else if (str.find(">=") != std::string::npos)
	{
		sign = ">=";
		return StateConditions::GreaterEqual;
	}
	else if (str.find("<") != std::string::npos)
	{
		sign = "<";
		return StateConditions::Smaller;
	}
	else if (str.find(">") != std::string::npos)
	{
		sign = ">";
		return StateConditions::Greater;
	}
	else if (str.find("==") != std::string::npos)
	{
		sign = "==";
		return StateConditions::Equal;
	}

    assert(false);
    return StateConditions();
}
