#pragma once

class TransformStructure;
class IKSolver
{
public:
							IKSolver();

	virtual void			createIKSolver(TransformStructure* effecter, TransformStructure* root, TransformStructure* target);
	virtual void			destroyIKSolver() = 0;
	virtual void			solve() = 0;

	TransformStructure*		getRoot() { return _root; }
	TransformStructure*		getTarget() { return _target; }
	TransformStructure*		getEffector() { return _effector; }
protected:
	TransformStructure*		_root;
	TransformStructure*		_target;
	TransformStructure*		_effector;
};

