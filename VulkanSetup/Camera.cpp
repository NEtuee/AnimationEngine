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

TransformStructure& Camera::GetTransform()
{
	return *transform;
}

void Camera::frame(float deltaTime)
{
	XMVECTOR pos = transform->getWorldPosition();
	XMFLOAT3 realPos;
	XMStoreFloat3(&realPos, pos);

	if (Input::getInstance()->isDown(VK_LEFT))
	{
		XMFLOAT3 right;
		XMStoreFloat3(&right, transform->getWorldTransform().getRight());
		right = MathEx::mul(right, 9.f * deltaTime);
		realPos = MathEx::sub(realPos, right);
	}
	else if (Input::getInstance()->isDown(VK_RIGHT))
	{
		XMFLOAT3 right;
		XMStoreFloat3(&right, transform->getWorldTransform().getRight());
		right = MathEx::mul(right, 9.f * deltaTime);
		realPos = MathEx::add(realPos, right);
	}
	else if (Input::getInstance()->isDown(VK_UP))
	{
		XMFLOAT3 forward;
		XMStoreFloat3(&forward, transform->getWorldTransform().getForward());
		forward = MathEx::mul(forward, 9.f * deltaTime);
		realPos = MathEx::add(realPos, forward);
	}
	else if (Input::getInstance()->isDown(VK_DOWN))
	{
		XMFLOAT3 forward;
		XMStoreFloat3(&forward, transform->getWorldTransform().getForward());
		forward = MathEx::mul(forward, 9.f * deltaTime);
		realPos = MathEx::sub(realPos, forward);
	}

	if (Input::getInstance()->isDown('Q'))
	{
		XMFLOAT3 right;
		XMStoreFloat3(&right, transform->getWorldTransform().getUp());
		right = MathEx::mul(right, 9.f * deltaTime);
		realPos = MathEx::add(realPos, right);
	}
	if (Input::getInstance()->isDown('A'))
	{
		XMFLOAT3 right;
		XMStoreFloat3(&right, transform->getWorldTransform().getUp());
		right = MathEx::mul(right, 9.f * deltaTime);
		realPos = MathEx::sub(realPos, right);
	}

	transform->SetWorldPosition(realPos);
	transform->SetWorldRotation(MathEx::LookAtQuaternion(transform->getWorldPosition(), XMVectorSet(0.f, realPos.y, 0.f, 0.f)));
}

void Camera::render()
{
	XMVECTOR upVector, positionVector, lookAtVector;

	positionVector = transform->getWorldTransform().getPosition();
	lookAtVector = positionVector + transform->getWorldTransform().getForward();
	upVector = transform->getWorldTransform().getUp();

	
	// 뷰 행렬 생성
	
	_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void Camera::getViewMatrix(XMMATRIX& view)
{
	view = _viewMatrix;
}
