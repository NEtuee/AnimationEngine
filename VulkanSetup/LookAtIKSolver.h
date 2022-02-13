#pragma once
#include "IKSolver.h"
#include "DirectXDefine.h"
#include <vector>

class TransformStructure;
class LookAtIKSolver : public IKSolver
{
public:
	virtual void						createIKSolver(TransformStructure* effecter, TransformStructure* root, TransformStructure* target) override;
	virtual void						destroyIKSolver()	override;
	virtual void						solve()				override;

	void								setOptionBone(TransformStructure* head, TransformStructure* forward);

	std::vector<TransformStructure*>*	getSpines()			{ return &_spines; }
	TransformStructure*					getHead()			{ return _head; }
	TransformStructure*					getLookForward()	{ return _lookForward; }
private:
	void								setRotationToTransform(TransformStructure* transform, float weight);
private:
	std::vector<TransformStructure*>	_spines;
	TransformStructure*					_head = nullptr;
	TransformStructure*					_lookForward = nullptr;

	float								_fixWeight	= 0.f;
	float								_minWeight	= 0.05f;
	float								_maxWeight	= 0.2f;
	float								_interval	= 0.f;
};

