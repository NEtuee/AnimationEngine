#include "../VulkanSetup/AnimationLoader.h"
#include "../VulkanSetup/AnimationDataRow.h"
#include "../VulkanSetup/FrameData.h"
#include "../VulkanSetup/XMLLoader.h"
#include "../VulkanSetup/XMLData.h"
#include "../VulkanSetup/StringEx.h"
#include "../VulkanSetup/Serialize.h"
#include "../VulkanSetup/HalfFloat.h"

#include <functional>
#include <iostream>
#include <fstream>
#include <vector>

#include "FBXLoader.h"
#include "../VulkanSetup/BoneStructure.h"
#include "../VulkanSetup/Serialize.h"
#include "../VulkanSetup/TransformStructure.h"
#include "../VulkanSetup/SkeletonLoader.h"

#include "../VulkanSetup/SplineKeyVariable.h"
#include "../VulkanSetup/CatmullRomSpline.h"

#include <filesystem>

int main()
{
	


	while (true)
	{
		std::string path;
		std::cout << "path : ";
		std::cin >> path;

		if (path.compare("exit") == 0)
			break;

		for (const auto& file : std::filesystem::directory_iterator(path))
		{
			if (file.path().extension().compare(".fbx") == 0)
			{
				AnimationLoader loader;
				FBXLoader fbxLoader;
				std::string path = file.path().string();
				//auto ani = loader.loadAnimation(path);
				auto ani = fbxLoader.loadAnimation(path.c_str());
				loader.optimization(*ani);

				std::ofstream save;
				path = path.substr(0, path.size() - 3) + ".dat";
				std::cout << path << std::endl;
				save.open(path, std::ios::binary);
				Serialization serialize;
				ani->serialize(&serialize, &save);

				save.close();
			}
		}
	}
	


	return 0;
}