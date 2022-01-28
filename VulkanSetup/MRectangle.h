#pragma once
#include "Model.h"

class MRectangle : public Model
{
public:
	virtual bool initialize(ID3D11Device* device) override;
};

