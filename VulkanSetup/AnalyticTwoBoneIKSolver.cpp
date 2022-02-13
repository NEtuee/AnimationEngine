#include "AnalyticTwoBoneIKSolver.h"
#include "TransformStructure.h"
#include "MathEx.h"


void AnalyticTwoBoneIKSolver::createIKSolver(TransformStructure* effecter, TransformStructure* root, TransformStructure* target)
{
	IKSolver::createIKSolver(effecter, root, target);
	_joints[0] = effecter;
	_joints[1] = effecter->getParent();
	_joints[2] = _joints[1]->getParent();

	_lowerDistance = XMVectorGetX(XMVector3Length(_joints[1]->getWorldPosition() - _joints[0]->getWorldPosition()));
	_upperDistance = XMVectorGetX(XMVector3Length(_joints[2]->getWorldPosition() - _joints[1]->getWorldPosition()));
	_completeLength = _lowerDistance + _upperDistance;
}

void AnalyticTwoBoneIKSolver::destroyIKSolver()
{
}

void AnalyticTwoBoneIKSolver::solve()
{
	XMVECTOR jointDir = XMVector3Normalize(_joints[1]->getWorldPosition() - _joints[2]->getWorldPosition());
	XMVECTOR endDir = XMVector3Normalize(_joints[0]->getWorldPosition() - _joints[1]->getWorldPosition());

	XMVECTOR effecterDir = (_target->getWorldPosition() - _joints[2]->getWorldPosition());
	float effecterDist = XMVectorGetX(XMVector3Length(effecterDir));
	effecterDir = XMVector3Normalize(effecterDir);

	XMVECTOR outEndPos = _target->getWorldPosition();
	XMVECTOR outJointPos = _joints[1]->getWorldPosition();

	if (effecterDist >= _completeLength)
	{
		outEndPos = _joints[2]->getWorldPosition() + (_completeLength * effecterDir);
		outJointPos = _joints[2]->getWorldPosition() + (_upperDistance * effecterDir);
	}
	else
	{
		XMVECTOR bendTargetDir = _bendTarget->getWorldPosition() - _joints[2]->getWorldPosition();
		float bendTargetDist = XMVectorGetX(XMVector3LengthSq(bendTargetDir));

		XMVECTOR jointPlaneNormal, jointBendDir;
		if (bendTargetDist < 0.00001f)
		{
			jointBendDir = XMVectorSet(0.f, 0.f, 1.f, 0.f);
			jointPlaneNormal = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		}
		else
		{
			jointPlaneNormal = XMVector3Cross(effecterDir, bendTargetDir);
			if (XMVectorGetX(XMVector3LengthSq(jointPlaneNormal)) < 0.00001f)
			{
				effecterDir = jointPlaneNormal;
				jointBendDir = XMVectorZero();
			}
			else
			{
				jointPlaneNormal = XMVector3Normalize(jointPlaneNormal);
				jointBendDir = bendTargetDir - (XMVectorGetX(XMVector3Dot(bendTargetDir, effecterDir)) * effecterDir);

				jointBendDir = XMVector3Normalize(jointBendDir);
			}
		}

		float cosDenominator = 2.f * _upperDistance * effecterDist;
		float cosAngle = ((_upperDistance * _upperDistance) + (effecterDist * effecterDist) - (_lowerDistance * _lowerDistance)) / cosDenominator;

		bool reverseUpperBone = cosAngle < 0.f;

		cosAngle = max(-1.f, min(1.f, cosAngle));
		float angle = acosf(cosAngle);
		float jointLineDist = _upperDistance * sinf(angle);

		float projJointDistSqr = (_upperDistance * _upperDistance) - (jointLineDist * jointLineDist);
		float projJointDist = (projJointDistSqr > 0.f ? sqrtf(projJointDistSqr) : 0.f);
		if (reverseUpperBone)
		{
			projJointDist *= -1.f;
		}

		outJointPos = _joints[2]->getWorldPosition() + (projJointDist * effecterDir) +(jointLineDist * jointBendDir);
	}

	XMVECTOR jointRotation = _joints[1]->getWorldRotation();

	_joints[2]->SetWorldRotation(XMQuaternionMultiply(_joints[2]->getWorldRotation(),
				MathEx::fromToQuaternion(jointDir, XMVector3Normalize(outJointPos - _joints[2]->getWorldPosition()))));

	_joints[1]->SetWorldRotation(XMQuaternionMultiply(jointRotation,
		MathEx::fromToQuaternion(endDir, XMVector3Normalize(outEndPos - outJointPos))));
	_joints[1]->SetWorldPosition(outJointPos);

	_joints[0]->SetWorldPosition(outEndPos);
}

void AnalyticTwoBoneIKSolver::setBendTarget(TransformStructure* target)
{
	_bendTarget = target;
}
