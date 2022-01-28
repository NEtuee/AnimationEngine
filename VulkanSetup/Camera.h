#pragma once
#include "DirectXDefine.h"

class TransformStructure;
class Camera : public AlignedAllocationPolicy<16>
{
public:
				Camera();
				~Camera();

	void		initialize();
	void		createCamera();
	void		destroyCamera();

	void		setPosition(float x, float y, float z);
	void		setRotation(float x, float y, float z);

	void		frame(float deltaTime);
	void		render();
	void		getViewMatrix(XMMATRIX& view);
	
	TransformStructure& GetTransform();
private:
	TransformStructure*		transform;
	XMMATRIX				_viewMatrix;
};

