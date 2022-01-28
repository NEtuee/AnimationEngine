#pragma once
#include "Scene.h"
#include "DirectXDefine.h"

class Model;
class Shader;
class GroundScene : public Scene
{
public:
							GroundScene();
							~GroundScene();

	void					setPosition(float x, float y, float z) { _position = XMVectorSet(x,y,z,0.f); };

	virtual void			createScene(Graphics*) override;
	virtual void			destroyScene() override;
	virtual void			frame(float) override;
	virtual bool			renderScene(Graphics*, Camera*) override;
private:
	Model*					_quad = nullptr;
	Shader*					_shader = nullptr;
	XMVECTOR				_position{ 0 };
};

