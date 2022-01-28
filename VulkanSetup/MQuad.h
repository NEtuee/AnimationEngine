#pragma once
#include "Model.h"

class MQuad : public Model
{
public:
	virtual bool	initialize(ID3D11Device* device) override;
	void			setValue(float size, int width, int height);
private:
	float			_rectSize = 1.f;
	int				_width = 10;
	int				_height = 10;
};

