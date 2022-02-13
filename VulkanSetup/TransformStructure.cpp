#include "TransformStructure.h"

TransformStructure::TransformStructure()
	:_children(0), _localTransform(Transform()), _worldTransform(Transform()), _parent(nullptr), _depth(0.f), _hashedName(0)
{
}


void TransformStructure::SetLocalPosition(const XMVECTOR& vec)
{
	_localTransform.setPosition(vec);
	updateWorld();
}

void TransformStructure::SetLocalRotation(const XMVECTOR& vec)
{
	_localTransform.setRotation(vec);
	updateWorld();
}

void TransformStructure::SetLocalScale(const XMVECTOR& vec)
{
	_localTransform.setScale(vec);
	updateWorld();
}

void TransformStructure::SetLocalPosition(const XMFLOAT3& f)
{
	_localTransform.setPosition(f);
	updateWorld();
}

void TransformStructure::SetLocalRotation(const XMFLOAT3& f)
{
	_localTransform.setRotation(f);
	updateWorld();
}

void TransformStructure::SetLocalScale(const XMFLOAT3& f)
{
	_localTransform.setScale(f);
	updateWorld();
}

void TransformStructure::SetWorldTransformNoCalc(const XMVECTOR& pos, const XMVECTOR& scale, const XMVECTOR& rotation)
{
	_worldTransform.setTransform(pos, scale, rotation);
}

void TransformStructure::SetLoaclTransformNoCalc(const XMVECTOR& pos, const XMVECTOR& scale, const XMVECTOR& rotation)
{
	_localTransform.setTransform(pos, scale, rotation);
}

void TransformStructure::SetWorldPosition(const XMVECTOR& vec)
{
	_worldTransform.setPosition(vec);
	updateLocal();
}

void TransformStructure::SetWorldRotation(const XMVECTOR& vec)
{
	_worldTransform.setRotation(vec);
	updateLocal();
}

void TransformStructure::SetWorldScale(const XMVECTOR& vec)
{
	_worldTransform.setScale(vec);
	updateLocal();
}

void TransformStructure::SetWorldPositionNoCalc(const XMVECTOR& vec)
{
	_worldTransform.setPosition(vec);
}

void TransformStructure::SetWorldRotationNoCalc(const XMVECTOR& vec)
{
	_worldTransform.setRotation(vec);
}

void TransformStructure::SetWorldScaleNoCalc(const XMVECTOR& vec)
{
	_worldTransform.setScale(vec);
}

void TransformStructure::SetWorldPosition(const XMFLOAT3& f)
{
	_worldTransform.setPosition(f);
	updateLocal();
}

void TransformStructure::SetWorldRotation(const XMFLOAT3& f)
{
	_worldTransform.setRotation(f);
	updateLocal();
}

void TransformStructure::SetWorldScale(const XMFLOAT3& f)
{
	_worldTransform.setScale(f);
	updateLocal();
}

Transform TransformStructure::localTransformLerp(const Transform& target, float factor)
{
	return _localTransform.lerp(target,factor);
}

Transform TransformStructure::worldTransformLerp(const Transform& target, float factor)
{
	return _worldTransform.lerp(target,factor);
}

int TransformStructure::getChildCountAll()
{
	return getChildCountAll(this,0);
}

TransformStructure* TransformStructure::getParent() const
{
	return _parent;
}

void TransformStructure::setParent(TransformStructure* parent)
{
	if (hasParent())
	{
		_parent->removeChild(this);
		updateLocal();
	}

	if (parent == nullptr)
		return;

	parent->addChild(this);
	_parent = parent;

	updateLocal();
}

bool TransformStructure::hasParent()
{
	return _parent != nullptr;
}

void TransformStructure::removeChild(TransformStructure* child)
{
	std::vector<TransformStructure*>::const_iterator iterator = std::find(_children.begin(), _children.end(), child);
	if (iterator == _children.end())
		return;

	_children.erase(iterator);
	child->setParentNull();
}

void TransformStructure::addChild(TransformStructure* child)
{
	_children.emplace_back(child);
}


void TransformStructure::updateLocal()
{
	if (hasParent())
	{
		_localTransform = _worldTransform.worldToLocal(_parent->getWorldTransform());
	}
	else
	{
		_localTransform = _worldTransform;
	}

	updateChildren();
}

void TransformStructure::updateWorld()
{
	if (hasParent())
	{
		_worldTransform = _localTransform.localToWorld(_parent->getWorldTransform());
	}
	else
	{
		_worldTransform = _localTransform;
	}

	updateChildren();
}

void TransformStructure::updateLocalSelf()
{
	if (hasParent())
	{
		_localTransform = _worldTransform.worldToLocal(_parent->getWorldTransform());
	}
	else
	{
		_localTransform = _worldTransform;
	}
}

void TransformStructure::updateWorldSelf()
{
	if (hasParent())
	{
		_worldTransform = _localTransform.localToWorld(_parent->getWorldTransform());
	}
	else
	{
		_worldTransform = _localTransform;
	}
}

TransformStructure* TransformStructure::copyStructure(TransformStructure* target)
{
	TransformStructure* transform = new TransformStructure;

	transform->_canDraw = target->_canDraw;
	transform->_localTransform = target->_localTransform;
	transform->_worldTransform = target->_worldTransform;
	transform->_depth = target->_depth;
	transform->_name = target->_name;
	transform->_hashedName = target->_hashedName;

	for (int i = 0; i < target->_children.size(); ++i)
	{
		TransformStructure* child = copyStructure(target->_children[i]);
		child->setParent(transform);
	}

	return transform;
}

TransformStructure* TransformStructure::copyThis()
{
	return copyStructure(this);
}

void TransformStructure::serialize(Serialization* serialize, std::ostream* stream)
{
	serialize->writeString(stream, _name);
	serialize->write(stream, &_worldTransform, sizeof(Transform));
	serialize->write(stream, &_localTransform, sizeof(Transform));
	serialize->write(stream, &_hashedName, sizeof(size_t));
	serialize->write(stream, &_depth, sizeof(float));
	serialize->write(stream, &_canDraw, sizeof(bool));

	unsigned short count = static_cast<unsigned short>(_children.size());
	serialize->write(stream, &count, sizeof(unsigned short));
	for (int i = 0; i < count; ++i)
	{
		_children[i]->serialize(serialize, stream);
	}

	//serialize->writeVector(stream, _children);
}

void TransformStructure::deserialize(Serialization* serialize, std::istream* stream)
{
	serialize->readString(stream, _name);
	serialize->read(stream, &_worldTransform, sizeof(Transform));
	serialize->read(stream, &_localTransform, sizeof(Transform));
	serialize->read(stream, &_hashedName, sizeof(size_t));
	serialize->read(stream, &_depth, sizeof(float));
	serialize->read(stream, &_canDraw, sizeof(bool));

	unsigned short count = static_cast<unsigned short>(_children.size());
	serialize->read(stream, &count, sizeof(unsigned short));
	for (int i = 0; i < count; ++i)
	{
		TransformStructure* transformStructure = new TransformStructure;
		transformStructure->deserialize(serialize, stream);

		transformStructure->_parent = this;
		_children.push_back(transformStructure);
	}
}

void TransformStructure::updateChildren()
{
	for (auto iter = _children.begin(); iter != _children.end(); ++iter)
	{
		(*iter)->updateWorld();
	}
}

int TransformStructure::getChildCountAll(TransformStructure* root, int count)
{
	auto children = root->getChildren();
	size_t save = 0;
	for (auto iter = children.begin(); iter != children.end(); ++iter)
	{
		save += getChildCountAll((*iter),count);
	}

	count += static_cast<int>(save + children.size());

	return count;
}
