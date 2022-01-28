#pragma once
#include <string>
#include <unordered_map>
#include <mutex>


class AnimationDataPack;
class BoneStructure;
class ThreadPool;
class ResourceLoader
{
public:
							ResourceLoader();

	static ResourceLoader*	getInstance();

	void					createResourceLoader();
	void					destroyResourceLoader();

	bool					jobDone();
	void					waitToLoad();


	AnimationDataPack*		getAnimation(std::string path);
	BoneStructure*			getSkeleton(std::string path);
	BoneStructure*			getSkeletonOrigin(std::string path);
	AnimationDataPack*		getSkeletonPose(std::string path);

	void					loadAnimation(std::string path);
	void					loadSkeleton(std::string path, std::string renderTarget);

	void					preloadAnimation(std::string path);
	void					preloadSkeleton(std::string path, std::string renderTarget);


	std::unordered_map<std::string, AnimationDataPack*>& getAnimations();
private:
	std::unordered_map<std::string, AnimationDataPack*> _animations;
	std::unordered_map<std::string, BoneStructure*>		_skeletons;
	std::unordered_map<std::string, AnimationDataPack*> _skeletonPoses;

	ThreadPool*		_threadPool;
	std::mutex		_mutex;
};

