#include "SkeletonLoader.h"
#include "TransformStructure.h"
#include "XMLLoader.h"
#include "XMLData.h"
#include "StringEx.h"
#include "MathEx.h"
#include "Serialize.h"

#include <iostream>
#include <fstream>
#include <time.h>
#include <utility>
#include <functional>


TransformStructure* SkeletonLoader::loadSkeleton(std::string path, std::string renderTarget)
{
	clock_t start, end;
	double result;
	start = clock();

	XMLLoader loader;
	XMLElement* data = loader.loadXML_c(path);
	data = data->findElement("Skeleton");
	if (data == nullptr)
	{
		std::cout << "can't find skeleton\n";
		return nullptr;
	}

	std::unordered_map<std::string, std::pair<TransformStructure*, XMLElement*>> bones;

	int count = std::stoi(data->getProperty("Count"));
	for (int i = 0; i < count; ++i)
	{
		if(data->_childs[i]->getProperty("Name").find("TL") != std::string::npos)
			continue;;

		TransformStructure* transform = createTransformFromData(data->_childs[i]);

		if (bones.find(transform->getName()) != bones.end())
		{
			std::cout << transform->getName() << std::endl;
			return nullptr;
		}

		bones[transform->getName()] = std::make_pair(transform, data->_childs[i]);

		if (renderTarget != "")
			transform->canDraw(transform->getName().find(renderTarget) != std::string::npos);
	}

	for (auto iter = bones.begin(); iter != bones.end(); ++iter)
	{
		XMLElement* childInfo = (*iter).second.second;
		childInfo = childInfo->findElement("Children");

		for (unsigned int i = 0; i < childInfo->_data.size(); ++i)
		{
			auto child = bones.find(childInfo->_data[i]->data);
			if (child == bones.end())
			{
				assert("child bone does not exists");
				return nullptr;
			}

			(*child).second.first->setParent((*iter).second.first);
		}
		
		
	}

	TransformStructure* mainRoot = new TransformStructure;
	mainRoot->setName("root");

	for (auto iter = bones.begin(); iter != bones.end(); ++iter)
	{
		TransformStructure* transform = (*iter).second.first;
		
		if (transform->getParent() == nullptr)
			transform->setParent(mainRoot);

	}

	data->destroyChildren();
	delete data;

	bones.clear();

	end = clock() - start;
	result = static_cast<double>(end);

	std::cout << "skeleton loaded : " << result << "ms" << std::endl;

	return mainRoot;
}

TransformStructure* SkeletonLoader::loadSkeletonBinary(std::string path)
{
	Serialization serialize;
	std::ifstream openFile;
	openFile.open(path, std::ios::binary);

	TransformStructure* tp = new TransformStructure;
	tp->deserialize(&serialize, &openFile);

	openFile.close();

	return tp;
}

TransformStructure* SkeletonLoader::createTransformFromData(XMLElement* data)
{
	XMLElement* transformData = data->findElement("Transform");
	XMFLOAT3 scale = StringEx::dataToFloat3(transformData->findData("Scale")->data);
	XMFLOAT4 rotation = StringEx::dataToFloat4(transformData->findData("Rotation")->data);
	XMFLOAT3 translation = MathEx::mul(StringEx::dataToFloat3(transformData->findData("Translation")->data),0.01f);

	MathEx::swap<float>(translation.z, translation.y);
	MathEx::swap<float>(rotation.z, rotation.y);

	std::hash<std::string> nameHash;

	TransformStructure* transform = new TransformStructure;
	transform->setName(data->getProperty("Name"));
	transform->setHashedName(nameHash(transform->getName()));
	transform->SetWorldPosition(translation);
	transform->SetWorldRotation(XMVectorSet(rotation.x, rotation.y, rotation.z, rotation.w));
	transform->SetWorldScale(scale);

	return transform;
}
