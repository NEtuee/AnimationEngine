#include "MTriangle.h"

bool MTriangle::initialize(ID3D11Device* device)
{
	VertexType* vertices = new VertexType[3];
	if (vertices == nullptr)
	{
		return false;
	}

	unsigned long* indices = new unsigned long[3];
	if (indices == nullptr)
	{
		return false;
	}

	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	bool result = initializeBuffers(device, vertices, indices, 3, 3);

	delete[] vertices;
	delete[] indices;

	return result;
}
