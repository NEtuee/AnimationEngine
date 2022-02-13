#include "CCDIKSolver.h"
#include "TransformStructure.h"
#include "MathEx.h"

void CCDIKSolver::createIKSolver(TransformStructure* effecter, TransformStructure* root, TransformStructure* target)
{
	IKSolver::createIKSolver(effecter, root, target);

	_deltaDistance = 0.001f;
	_iteration = 10;

	int loopCount = 0;
	TransformStructure* current = effecter;

	while (current != root->getParent())
	{
		_joints.push_back(current);
		current = current->getParent();
	}
}

void CCDIKSolver::destroyIKSolver()
{
}

void CCDIKSolver::solve()
{
	XMVECTOR target = _target->getWorldPosition();

	for (int iter = 0; iter < _iteration; ++iter)
	{
		for (int i = 1; i < _joints.size(); ++i)
		{
			XMVECTOR start = _joints[0]->getWorldPosition();
			XMVECTOR toStart = XMVector3Normalize(start - _joints[i]->getWorldPosition());
			XMVECTOR toEffecter = XMVector3Normalize(target - _joints[i]->getWorldPosition());

			_joints[i]->SetWorldRotation(
				XMQuaternionMultiply(_joints[i]->getWorldRotation(), MathEx::fromToQuaternion(toStart, toEffecter))
			);

			if (XMVectorGetX(XMVector3LengthSq(_joints[0]->getWorldPosition() - _target->getWorldPosition())) <= _deltaDistance * _deltaDistance)
			{
				break;
			}
		}

	}
}
