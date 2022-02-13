#include "FABRIKSolver.h"
#include "TransformStructure.h"
#include "MathEx.h"

void FABRIKSolver::createIKSolver(TransformStructure* effecter, TransformStructure* root, TransformStructure* target)
{
	IKSolver::createIKSolver(effecter, root, target);

	_deltaDistance = 0.0001f;
	_iteration = 10;

	int loopCount = 0;
	TransformStructure* current = root;

	_startRotationTarget = _target->getWorldRotation();

	while (current != _root->getParent())
	{
		_joints.push_back(current);
		_positions.push_back(current->getWorldPosition());
		_startRotations.push_back(XMQuaternionNormalize(current->getWorldRotation()));

		current = current->getParent();
		if (current != nullptr)
		{
			_length.push_back(XMVectorGetX(XMVector3Length(_joints[loopCount]->getWorldPosition() - current->getWorldPosition())));
			_startDirections.push_back(XMVector3Normalize(_joints[loopCount]->getWorldPosition() - current->getWorldPosition()));
			_completeLength += _length[loopCount];
		}

		loopCount++;

		
	}
}

void FABRIKSolver::destroyIKSolver()
{
}

void FABRIKSolver::solve()
{
	for (size_t i = 0; i < _positions.size(); ++i)
	{
		_positions[i] = _joints[i]->getWorldPosition();
	}

	XMVECTOR target = _target->getWorldPosition();
	XMVECTOR len = XMVector3Length(target - _joints[_joints.size() - 1]->getWorldPosition());

	if (XMVectorGetX(len) >= _completeLength) //solve 1
	{
		XMVECTOR dir = XMVector3Normalize(target - _joints[_joints.size() - 1]->getWorldPosition());

		for (size_t i = _joints.size() - 2; i >= 0; --i)
		{
			_positions[i] = (_positions[i + 1] + dir * _length[i]);
		}
	}
	else //solve 2
	{
		for (int iter = 0; iter < _iteration; ++iter)
		{

			for (size_t i = 0; i < _joints.size() - 1; ++i)
			{
				if (i == 0)
				{
					_positions[i] = (target);
				}
				else
				{
					_positions[i] = (_positions[i - 1] +
						XMVector3Normalize(_positions[i] - _positions[i - 1]) * _length[i - 1]);
				}
			}

			for (size_t i = _joints.size() - 2; i >= 0; --i)
			{
				_positions[i] = (_positions[i + 1] +
					XMVector3Normalize(_positions[i] - _positions[i + 1]) * _length[i]);
			}

			if (XMVectorGetX(XMVector3LengthSq(_positions[0] - _target->getWorldPosition())) <= _deltaDistance * _deltaDistance)
				break;
		}

	}

	for (size_t i = _joints.size() - 1; i > 0; --i)
	{
		if (i == 0)
		{
			_joints[i]->SetWorldRotation(XMQuaternionInverse(
				XMQuaternionMultiply(
					XMQuaternionMultiply(_target->getWorldRotation(), _startRotationTarget),
					XMQuaternionInverse(_startRotations[i])))
				);
		}
		else
		{
			XMVECTOR from = _startDirections[i-1];
			XMVECTOR to = XMVector3Normalize(_positions[i - 1] - _positions[i]);

			_joints[i]->SetWorldRotation(XMQuaternionMultiply(MathEx::fromToQuaternion(from,to), (_startRotations[i])));
		}

	}

	for (size_t i = _joints.size() - 1; i >= 0; --i)
	{
		_joints[i]->SetWorldPosition(_positions[i]);

	}
}
