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

int main()
{
	//FBXLoader loader;
	//loader.loadAnimation("../Resource/Player_Run_02.fbx", "Check");

	/*BoneStructure* bone = loader.loadBone("../Resource/mesh_player_1016.fbx");
	bone->getRootBone();
	Serialization seri;

	std::ofstream file;
	file.open("../Resource/bone.dat", std::ios::binary);
	bone->getRootBone().serialize(&seri, &file);

	file.close();

	BoneStructure* newBone;

	TransformStructure* tp = new TransformStructure;

	std::ifstream openFile;
	openFile.open("../Resource/bone.dat", std::ios::binary);
	tp->deserialize(&seri, &openFile);
	newBone = new BoneStructure(tp);
	openFile.close();*/

	//std::ifstream file;
	//file.open(path, std::ios::binary);

	//AnimationDataRow* row = new AnimationDataRow;
	//Serialization serialization;

	//row->deserialize(&serialization, &file);

	//return 0;


	//std::cout << sizeof(FrameData) << "\n";
	while (true)
	{
		std::string path;
		std::cout << "path : ";
		std::cin >> path;

		if (path.compare("exit") == 0)
			break;

		AnimationLoader loader;
		FBXLoader fbxLoader;
		//auto ani = loader.loadAnimation(path);
		auto ani = fbxLoader.loadAnimation(path.c_str());
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