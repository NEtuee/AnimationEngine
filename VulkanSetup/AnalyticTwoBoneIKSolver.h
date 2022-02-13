#pragma once
#include "IKSolver.h"
#include "DirectXDefine.h"
#include <vector>

class TransformStructure;
class AnalyticTwoBoneIKSolver : public IKSolver
{
public:
	virtual void		createIKSolver(TransformStructure* effecter, TransformStructure* root, TransformStructure* target) override;
	virtual void		destroyIKSolver() override;
	virtual void		solve() override;

	void				setBendTarget(TransformStructure* target);
private:
	TransformStructure* _joints[3];

	TransformStructure* _bendTarget;

	float				_lowerDistance;
	float				_upperDistance;

	float				_completeLength;
	float				_deltaDistance;
	int					_iteration;
};

