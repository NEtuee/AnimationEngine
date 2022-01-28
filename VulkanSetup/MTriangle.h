#pragma once
#include "Model.h"

class MTriangle : public Model
{
public:
	virtual bool initialize(ID3D11Device* device) override;
};

