#include "LookAtIKSolver.h"
#include "TransformStructure.h"
#include "MathEx.h"
#include "Timer.h"

#include <iostream>
#include <math.h>

void LookAtIKSolver::createIKSolver(TransformStructure* effecter, TransformStructure* root, TransformStructure* target)
{
	IKSolver::createIKSolver(effecter, root, target);

	TransformStructure* current = effecter;
	while (current != root->getParent())
	{
		_spines.insert(_spines.begin(), current);
		current = current->getParent();
	}

	_interval = (_maxWeight - _minWeight) * (1.f / static_cast<float>(_spines.size()));
}

void LookAtIKSolver::destroyIKSolver()
{
}

void LookAtIKSolver::solve()
{
	float angle = XMConvertToDegrees(
		XMVectorGetX(XMVector3AngleBetweenNormals(_lookForward->getWorldTransform().getUp(),
		XMVector3Normalize(_lookForward->getWorldPosition() - _target->getWorldPosition())))
	);

	_fixWeight = MathEx::lerp(_fixWeight, (angle <= 90.f ? 1.f : 0.f), 0.15f);

	if (MathEx::similar(_fixWeight, 0.f, 0.0001f))
		return;

	for (int i = 0; i < _spines.size(); ++i)
	{
		float weight = _minWeight + (static_cast<float>(i) * _interval);
		setRotationToTransform(_spines[i], weight);
	}

	setRotationToTransform(_head, 1.f);
	_spines[0]->updateLocal();
}

void LookAtIKSolver::setOptionBone(TransformStructure* head, TransformStructure* forward)
{
	_head = head;
	_lookForward = forward;
}

void LookAtIKSolver::setRotationToTransform(TransformStructure* transform, float weight)
{
	transform->updateWorldSelf();

	XMVECTOR forward = transform->getWorldTransform().getUp();
	XMVECTOR targetDirection = XMVector3Normalize(_target->getWorldPosition() - transform->getWorldPosition());
	XMVECTOR targetQuaternion = MathEx::fromToQuaternion(forward, targetDirection,_fixWeight);

	if (weight == 1.f)
	{
		targetQuaternion = XMQuaternionMultiply(transform->getWorldRotation(), targetQuaternion);
	}
	else
		targetQuaternion = XMQuaternionMultiply(transform->getWorldRotation(), XMQuaternionSlerp(XMQuaternionIdentity(), targetQuaternion, weight));
	
	transform->SetWorldRotationNoCalc(targetQuaternion);
	transform->updateLocalSelf();
	
}
