#include "Camera.h"
#include "TransformStructure.h"
#include "MathEx.h"
#include "Input.h"

Camera::Camera()
	: _viewMatrix(XMMatrixIdentity()), transform(nullptr)
{
	
}

Camera::~Camera()
{
}

void Camera::initialize()
{
	transform->SetWorldPosition(XMFLOAT3(0.f, 0.f, -10.f));
}

void Camera::createCamera()
{
	transform = new TransformStructure;
}

void Camera::destroyCamera()
{
	delete transform;
}

void Camera::setPosition(float x, float y, float z)
{
	transform->SetWorldPosition(XMFLOAT3(x, y, z));
}

void Camera::setRotation(float x, float y, float z)
{
	transform->SetWorldRotation(XMFLOAT3(x, y, z));
}

TransformStructure* Camera::GetTransform()
{
	return transform;
}

void Camera::frame(float deltaTime)
{
	XMVECTOR pos = transform->getWorldPosition();
	XMFLOAT3 realPos;
	XMStoreFloat3(&realPos, pos);
	

	if (Input::getInstance()->isDown(VK_LEFT))
	{
		transform->SetWorldRotation(XMQuaternionMultiply(transform->getWorldRotation(), XMQuaternionRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), -deltaTime)));
	}
	if (Input::getInstance()->isDown(VK_RIGHT))
	{
		transform->SetWorldRotation(XMQuaternionMultiply(transform->getWorldRotation(), XMQuaternionRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), deltaTime)));
	}
	if (Input::getInstance()->isDown(VK_UP))
	{
		transform->SetWorldRotation(XMQuaternionMultiply(transform->getWorldRotation(), XMQuaternionRotationAxis(transform->getWorldTransform().getRight(), -deltaTime)));
	}
	if (Input::getInstance()->isDown(VK_DOWN))
	{
		transform->SetWorldRotation(XMQuaternionMultiply(transform->getWorldRotation(), XMQuaternionRotationAxis(transform->getWorldTransform().getRight(), deltaTime)));
	}

	if (Input::getInstance()->isDown('W'))
	{
		XMFLOAT3 forward;
		XMStoreFloat3(&forward, transform->getWorldTransform().getForward());
		forward = MathEx::mul(forward, 9.f * deltaTime);
		realPos = MathEx::add(realPos, forward);
	}
	if (Input::getInstance()->isDown('S'))
	{
		XMFLOAT3 forward;
		XMStoreFloat3(&forward, transform->getWorldTransform().getForward());
		forward = MathEx::mul(forward, 9.f * deltaTime);
		realPos = MathEx::sub(realPos, forward);
	}
	if (Input::getInstance()->isDown('A'))
	{
		XMFLOAT3 right;
		XMStoreFloat3(&right, transform->getWorldTransform().getRight());
		right = MathEx::mul(right, -9.f * deltaTime);
		realPos = MathEx::add(realPos, right);
	}
	if (Input::getInstance()->isDown('D'))
	{
		XMFLOAT3 right;
		XMStoreFloat3(&right, transform->getWorldTransform().getRight());
		right = MathEx::mul(right, 9.f * deltaTime);
		realPos = MathEx::add(realPos, right);
	}


	transform->SetWorldPosition(realPos);
}

void Camera::render()
{
	XMVECTOR upVector, positionVector, lookAtVector;

	positionVector = transform->getWorldTransform().getPosition();
	lookAtVector = positionVector + transform->getWorldTransform().getForward();
	upVector = transform->getWorldTransform().getUp();

	_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void Camera::getViewMatrix(XMMATRIX& view)
{
	view = _viewMatrix;
}
