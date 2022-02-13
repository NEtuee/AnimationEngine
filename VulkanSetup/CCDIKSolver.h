#pragma once
#include "IKSolver.h"
#include "DirectXDefine.h"
#include <vector>

class TransformStructure;
class CCDIKSolver : public IKSolver
{
public:
	virtual void		createIKSolver(TransformStructure* effecter, TransformStructure* root, TransformStructure* target) override;
	virtual void		destroyIKSolver() override;
	virtual void		solve() override;
private:
	std::vector<TransformStructure*> _joints;

	float				_completeLength;
	float				_deltaDistance;
	int					_iteration;
};

