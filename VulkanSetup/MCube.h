#pragma once
#include "Model.h"
#include "DirectXDefine.h"

class MCube : public Model
{
public:
	virtual bool initialize(ID3D11Device* device) override;
	void setColor(XMFLOAT4 color);
private:
	XMFLOAT4 _color;
};

