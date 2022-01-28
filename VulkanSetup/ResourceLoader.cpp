#include "ResourceLoader.h"
#include "AnimationDataPack.h"
#include "AnimationLoader.h"
#include "BoneStructure.h"
#include "SkeletonLoader.h"
#include "TransformStructure.h"
#include "ThreadPool.h"
#include "StringEx.h"

#include <string>
#include <functional>

ResourceLoader::ResourceLoader()
	:_threadPool(nullptr)
{
}

ResourceLoader* ResourceLoader::getInstance()
{
	static ResourceLoader instance;
	return &instance;
}

void ResourceLoader::createResourceLoader()
{
	_threadPool = new ThreadPool(5);
}

void ResourceLoader::destroyResourceLoader()
{
	for (auto iter = _animations.begin(); iter != _animations.end(); ++iter)
	{
		(*iter).second->destroyAnimationDataPack();
		delete (*iter).second;
	}

	for (auto iter = _skeletons.begin(); iter != _skeletons.end(); ++iter)
	{
		(*iter).second->destroyBoneStructure();
		delete (*iter).second;
	}
}

bool ResourceLoader::jobDone()
{
	return _threadPool->jobDone();
}

void ResourceLoader::waitToLoad()
{
	while (!_threadPool->jobDone());
}

AnimationDataPack* ResourceLoader::getAnimation(std::string path)
{
    auto find = _animations.find(path);
    if (find != _animations.end())
        return (*find).second;

	loadAnimation(path);

    return _animations[path];
}

BoneStructure* ResourceLoader::getSkeletonOrigin(std::string path)
{
	auto find = _skeletons.find(path);
	if (find != _skeletons.end())
		return (*find).second;

	loadSkeleton(path, "");

	return _skeletons[path];
}

AnimationDataPack* ResourceLoader::getSkeletonPose(std::string path)
{
	auto find = _skeletonPoses.find(path);
	if (find != _skeletonPoses.end())
		return (*find).second;

	return nullptr;
}

BoneStructure* ResourceLoader::getSkeleton(std::string path)
{
	auto find = _skeletons.find(path);
	if (find != _skeletons.end())
		return new BoneStructure(*(*find).second);

	loadSkeleton(path, "");

	//복사해서 보내삼
	return new BoneStructure(*_skeletons[path]);
}

void ResourceLoader::loadAnimation(std::string path)
{
	AnimationLoader loader;
	AnimationDataPack* dataPack = loader.loadDataPack(path);

	_mutex.lock();
	_animations.emplace(path, dataPack);
	_mutex.unlock();
}

void ResourceLoader::loadSkeleton(std::string path, std::string renderTarget)
{
	SkeletonLoader loader;
	TransformStructure* structure = loader.loadSkeleton(path, renderTarget);
	BoneStructure* boneStructure = new BoneStructure(structure);

	_mutex.lock();
	_skeletons.emplace(path, boneStructure);
	_mutex.unlock();

	AnimationDataPack* pose = new AnimationDataPack;
	pose->createAnimationDataPack(&boneStructure->getRootBone(), path);

	_mutex.lock();
	_skeletonPoses.emplace(path, pose);
	_mutex.unlock();
}

void ResourceLoader::preloadAnimation(std::string path)
{
	auto find = _animations.find(path);
	if (find != _animations.end())
		return;

	path = StringEx::replaceAll(path, "/", "\\");
	_threadPool->enqueueJob([](ResourceLoader* loader, std::string path)
		{
			loader->loadAnimation(path);
		},this,path);
}

void ResourceLoader::preloadSkeleton(std::string path, std::string renderTarget)
{
}

std::unordered_map<std::string, AnimationDataPack*>& ResourceLoader::getAnimations()
{
	return _animations;
}
