#include "IKChain.h"
#include "IKSolver.h"
#include "TransformStructure.h"

void IKChain::createIKChain(TransformStructure* effecter, TransformStructure* root, TransformStructure* target, IKSolver* solver)
{
	_root = root;
	_target = target;
	_effector = effecter;

	_solver = solver;
	_solver->createIKSolver(_effector, _root, _target);
}

void IKChain::destroyIKChain()
{
	_solver->destroyIKSolver();
	delete _solver;
}

void IKChain::solve()
{
	_solver->solve();
}
