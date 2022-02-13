#include "AnimationLoader.h"
#include <vector>
#include "StringEx.h"
#include "MathEx.h"
#include "AnimationDataRow.h"
#include "AnimationDataPack.h"
#include "XMLLoader.h"
#include "XmlData.h"
#include <functional>

void AnimationLoader::optimization(AnimationDataRow& data)
{
	auto& bones = data._hashBones.getTable();
	size_t prevCount = 0;
	size_t currCount = 0;

	for (int i = 0; i < bones.size(); ++i)
	{
		for (int j = 0; j < bones[i].size(); ++j)
		{
			int compareTarget = 0;
			auto& frames = bones[i][j]._data._frames;
			prevCount += frames.size();
			for (size_t i = 1; i < frames.size() - 1;)
			{

				if (frames[i].similar(frames[i - 1], 0.00001f) &&
					frames[i].similar(frames[i + 1], 0.00001f))
				{
					frames.erase(frames.begin() + i);
				}
				else
				{
					++i;
				}
			}

		}
	}

	//for (auto iterator = bones.begin(); iterator != bones.end(); ++iterator)
	//{
	//	int compareTarget = 0;
	//	auto& frames = (*iterator).second._frames;
	//	prevCount += frames.size();
	//	for (size_t i = 1; i < frames.size() - 1;)
	//	{

	//		if (frames[i].similar(frames[i - 1], 0.00001f) &&
	//			frames[i].similar(frames[i + 1], 0.00001f))
	//		{
	//			frames.erase(frames.begin() + i);
	//		}
	//		else
	//		{
	//			++i;
	//		}
	//	}


	//}

	for (int i = 0; i < bones.size(); ++i)
	{
		for (int j = 0; j < bones[i].size(); ++j)
		{
			size_t rootIndex = 0;
			auto& frames = bones[i][j]._data._frames;
			for (size_t i = 2; i < frames.size() - 1; ++i)
			{

				if (MathEx::linear((frames[i - 1].getPositionVector() - frames[rootIndex].getPositionVector()),
					(frames[i].getPositionVector() - frames[i - 1].getPositionVector()), 0.000001f) == false)
				{
					rootIndex = i - 1;
					continue;
				}

				if (MathEx::linear((frames[i - 1].getScaleVector() - frames[rootIndex].getScaleVector()),
					(frames[i].getScaleVector() - frames[i - 1].getScaleVector()), 0.000001f) == false)
				{
					rootIndex = i - 1;
					continue;
				}

				XMVECTOR forward = XMVectorSet(0.f, 0.f, 1.f, 0.f);
				if (MathEx::linear((frames[i - 1].getRotationVector() * forward - frames[rootIndex].getRotationVector() * forward),
					(frames[i].getRotationVector() * forward - frames[i - 1].getRotationVector() * forward), 0.000001f) == false)
				{
					rootIndex = i - 1;
					continue;
				}
				else
				{
					frames.erase(frames.begin() + (i - 1));
				}

			}

			currCount += frames.size();

		}
	}

	//for (auto iterator = bones.begin(); iterator != bones.end(); ++iterator)
	//{
	//	int rootIndex = 0;
	//	auto& frames = (*iterator).second._frames;
	//	for (size_t i = 2; i < frames.size() - 1; ++i)
	//	{

	//		if (MathEx::linear((frames[i - 1].getPositionVector() - frames[rootIndex].getPositionVector()),
	//							(frames[i].getPositionVector() - frames[i - 1].getPositionVector()), 0.000001f) == false)
	//		{
	//			rootIndex = i - 1;
	//			continue;
	//		}

	//		if (MathEx::linear((frames[i - 1].getScaleVector() - frames[rootIndex].getScaleVector()),
	//							(frames[i].getScaleVector() - frames[i - 1].getScaleVector()), 0.000001f) == false)
	//		{
	//			rootIndex = i - 1;
	//			continue;
	//		}

	//		XMVECTOR forward = XMVectorSet(0.f,0.f,1.f,0.f);
	//		if (MathEx::linear((frames[i - 1].getRotationVector() * forward - frames[rootIndex].getRotationVector() * forward),
	//							(frames[i].getRotationVector() * forward - frames[i - 1].getRotationVector() * forward), 0.000001f) == false)
	//		{
	//			rootIndex = i - 1;
	//			continue;
	//		}
	//		else
	//		{
	//			frames.erase(frames.begin() + (i - 1));
	//		}

	//	}

	//	currCount += frames.size();
	//}

	std::cout << "prev : " << prevCount << "\ncurr : " << currCount << "\n";
}


AnimationDataPack* AnimationLoader::loadDataPack(std::string path)
{
	AnimationDataRow* row = loadAnimationBinary(path);
	AnimationDataPack* pack = new AnimationDataPack;
	pack->createAnimationDataPack(row, path.substr(path.rfind('/') + 1, path.rfind('.')));
	return pack;
}

AnimationDataRow* AnimationLoader::loadAnimation(std::string path)
{
	XMLLoader loader;
	XMLElement* xml = loader.loadXML_c(path);
	AnimationDataRow* data = new AnimationDataRow;
	
	clock_t start, end;
	double result;
	start = clock();

	xml = xml->findElement("Animation");
	auto targetData = xml->_childs;

	data->_fps = stoi(xml->getProperty("FrameRate"));
	data->_frameCount = stoi(xml->getProperty("NumFrames"));
	data->_boneConut = stoi(xml->getProperty("NumNodes"));

	auto bs = xml->getProperty("BoneScale");
	if (bs != "")
		data->_boneScale = stof(xml->getProperty("BoneScale"));
	else
		data->_boneScale = 1.f;

	data->_isFacial = xml->getProperty("FacialAnimation") == "True";
	data->_isAdditive = xml->getProperty("AdditiveAnimation") == "True";
	data->_isSinglebone = xml->getProperty("SingleBoneAnimation") == "True";
	data->_fileTag = xml->getProperty("FileTag");

	//data->_bones.clear();
	data->_hashBones.createSimpleHashtable(data->_frameCount);
	//data->_frames.resize(data->_frameCount);

	for (int i = 0; i < targetData.size(); ++i)
	{
		auto node = targetData[i]->_childs;
		std::string name = targetData[i]->getProperty("Name");
		std::hash<std::string> hash;
		size_t hashedName = hash(name);

		BoneDataRow bone;
		bone._frames.resize(data->_frameCount);
		bone._name = name;
		//data->_bones.emplace(std::make_pair(hashedName,bone));
		data->_hashBones.pushData(hashedName, bone);

		for (int j = 0; j < node.size(); ++j)
		{
			int index = stoi(node[j]->getProperty("Index"));
			XMFLOAT3 scale = StringEx::dataToFloat3(node[j]->findData("S")->data);
			XMFLOAT3 position = StringEx::dataToFloat3(node[j]->findData("T")->data);
			XMFLOAT4 rotation = StringEx::dataToFloat4(node[j]->findData("R")->data);

			FrameData frame;
			frame.frame = index;
			frame.setPosition(position.x * 0.01f, position.z * 0.01f, position.y * 0.01f);
			frame.setScale(scale.x, scale.y, scale.z);
			frame.setRotation(rotation.x, rotation.z, rotation.y, rotation.w);

			BoneDataRow* row;
			data->_hashBones.find(hashedName, row);
			row->_frames[j] = frame;
			//data->_bones[hashedName]._frames[j] = frame;
		}
	}

	end = clock() - start;
	result = static_cast<double>(end);

	std::cout << "animation loaded : \"" << path << "\" " << result << "ms" << std::endl;

	xml->destroyChildren();

	return data;
}

AnimationDataRow* AnimationLoader::loadAnimationBinary(std::string path)
{
	std::ifstream file;
	file.open(path,std::ios::binary);

	AnimationDataRow* row = new AnimationDataRow;
	Serialization serialization;

	row->deserialize(&serialization, &file);

	return row;
}
