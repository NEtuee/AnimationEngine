#pragma once

class TransformStructure;
class IKSolver;
class IKChain
{
public:
	void				createIKChain(TransformStructure* effecter, TransformStructure* root, TransformStructure* target, IKSolver* solver);
	void				destroyIKChain();
	void				solve();
	IKSolver*			getSolver() { return _solver; }
private:
	TransformStructure* _root;
	TransformStructure* _target;
	TransformStructure* _effector;

	IKSolver*			_solver;
};

