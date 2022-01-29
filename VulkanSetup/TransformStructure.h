#pragma once
#include "Transform.h"
#include "Serialize.h"
#include <vector>
#include <string>

class AnimationPlayer;
class TransformStructure
{
public:
							TransformStructure();

	void					SetLocalPosition(const XMVECTOR& vec);
	void					SetLocalRotation(const XMVECTOR& vec);
	void					SetLocalScale(const XMVECTOR& vec);

	void					SetLocalPosition(const XMFLOAT3& f);
	void					SetLocalRotation(const XMFLOAT3& f);
	void					SetLocalScale(const XMFLOAT3& f);



	void					SetWorldPosition(const XMVECTOR& vec);
	void					SetWorldRotation(const XMVECTOR& vec);
	void					SetWorldScale(const XMVECTOR& vec);

	void					SetWorldPosition(const XMFLOAT3& f);
	void					SetWorldRotation(const XMFLOAT3& f);
	void					SetWorldScale(const XMFLOAT3& f);


	void					SetWorldTransformNoCalc(const XMVECTOR& pos, const XMVECTOR& scale, const XMVECTOR& rotation);
	void					SetLoaclTransformNoCalc(const XMVECTOR& pos, const XMVECTOR& scale, const XMVECTOR& rotation);


	XMVECTOR				getLocalPosition() { return getLocalTransform().getPosition(); }
	XMVECTOR				getLocalRotation() { return getLocalTransform().getRotation(); }
	XMVECTOR				getLocalScale() { return getLocalTransform().getScale(); }
		
	XMVECTOR				getWorldPosition() { return getWorldTransform().getPosition(); }
	XMVECTOR				getWorldRotation() { return getWorldTransform().getRotation(); }
	XMVECTOR				getWorldScale() { return getWorldTransform().getScale(); }

	Transform				localTransformLerp(const Transform& target, float factor);
	Transform				worldTransformLerp(const Transform& target, float factor);


	int						getChildCountAll();
public:
	TransformStructure*		getParent() const;
	void					setParent(TransformStructure* parent);
	void					setParentNull() { _parent = nullptr; }

	bool					hasParent();
	void					removeChild(TransformStructure* child);
	void					addChild(TransformStructure* child);

	XMMATRIX				getMatrix() { return _worldTransform.getMatrix(); } 

	void					setName(const std::string name) { _name = name; }
	void					setHashedName(size_t name) { _hashedName = name; }
	void					setDepth(float depth) { _depth = depth; }
	void					canDraw(bool value) { _canDraw = value; }


	void					updateLocal();
	void					updateWorld();

	const std::string&		getName() { return _name; }
	const size_t			getHashedName() { return _hashedName; }
	const Transform&		getWorldTransform() { return _worldTransform; }
	const Transform&		getLocalTransform() { return _localTransform; }
	bool					isCanDraw() { return _canDraw; }
	float					getDepth() { return _depth; }
	const std::vector<TransformStructure*>& getChildren() { return _children; }
	TransformStructure*		copyThis();
public:
	void serialize(Serialization* serialize, std::ostream* stream);
	void deserialize(Serialization* serialize, std::istream* stream);
private:
	void					updateChildren();
	TransformStructure*		copyStructure(TransformStructure* target);
	int						getChildCountAll(TransformStructure* root, int count);
private:
	std::string							_name;
	std::vector<TransformStructure*>	_children;
	Transform							_worldTransform;
	Transform							_localTransform;
	TransformStructure*					_parent;

	size_t								_hashedName;

	float								_depth;

	bool								_canDraw = true;
};

