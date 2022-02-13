#include "IKSolver.h"
#include "TransformStructure.h"

IKSolver::IKSolver()
:_root(nullptr), _target(nullptr), _effector(nullptr)
{
}

void IKSolver::createIKSolver(TransformStructure* effecter, TransformStructure* root, TransformStructure* target)
{
	_root = root;
	_target = target;
	_effector = effecter;
}
