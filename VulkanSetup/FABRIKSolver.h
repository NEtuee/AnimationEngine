#pragma once
#include "IKSolver.h"
#include "DirectXDefine.h"
#include <vector>

class TransformStructure;
class FABRIKSolver : public IKSolver
{
public:
	virtual void			createIKSolver(TransformStructure* effecter, TransformStructure* root, TransformStructure* target) override;
	virtual void			destroyIKSolver() override;
	virtual void			solve() override;
private:
	std::vector<TransformStructure*> _joints;
	std::vector<XMVECTOR>	_positions;
	std::vector<XMVECTOR>	_startRotations;
	std::vector<XMVECTOR>	_startDirections;
	std::vector<float>		_length;

	XMVECTOR				_startRotationTarget;

	float					_completeLength;
	float					_deltaDistance;
	int						_iteration;
};

