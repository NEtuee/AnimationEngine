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

int main()
{
	std::cout << sizeof(FrameData) << "\n";
	while (true)
	{
		std::string path;
		std::cout << "path : ";
		std::cin >> path;

		if (path.compare("exit") == 0)
			break;

		AnimationLoader loader;
		auto ani = loader.loadAnimation(path);
		loader.optimization(*ani);

		std::ofstream save;
		path = path.substr(0,path.size() - 3)+ ".dat";
		std::cout << path << std::endl;
		save.open(path, std::ios::binary);
		Serialization serialize;
		ani->serialize(&serialize, &save);

		save.close();
	}
	


	return 0;
}