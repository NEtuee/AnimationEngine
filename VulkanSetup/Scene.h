#pragma once

class Graphics;
class Camera;
class Scene
{
public:
					Scene() {}
					~Scene() {}

	virtual void	createScene(Graphics*) = 0;
	virtual void	destroyScene() = 0;
	virtual void	frame(float) = 0;
	virtual bool	renderScene(Graphics*, Camera*) = 0;
};

