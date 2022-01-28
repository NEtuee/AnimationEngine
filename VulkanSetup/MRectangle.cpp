#include "MRectangle.h"

bool MRectangle::initialize(ID3D11Device* device)
{
	VertexType* vertices = new VertexType[4];
	if (vertices == nullptr)
	{
		return false;
	}

	unsigned long* indices = new unsigned long[6];
	if (indices == nullptr)
	{
		return false;
	}

	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[2].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	vertices[3].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[3].color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;

	bool result = initializeBuffers(device, vertices, indices, 4, 6);

	delete[] vertices;
	delete[] indices;

	return result;
}
