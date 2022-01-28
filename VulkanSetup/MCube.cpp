#include "MCube.h"
#include "DirectXDefine.h"

bool MCube::initialize(ID3D11Device* device)
{
	VertexType* vertices = new VertexType[8];
	if (vertices == nullptr)
	{
		return false;
	}

	unsigned long* indices = new unsigned long[36];
	if (indices == nullptr)
	{
		return false;
	}

#pragma region Cube
	vertices[0].position = XMFLOAT3(-0.5f, -0.5f, 0.5f);
	vertices[0].color = _color;

	vertices[1].position = XMFLOAT3(-0.5f, 0.5f, 0.5f);
	vertices[1].color = _color;

	vertices[2].position = XMFLOAT3(0.5f, 0.5f, 0.5f);
	vertices[2].color = _color;

	vertices[3].position = XMFLOAT3(0.5f, -0.5f, 0.5f);
	vertices[3].color = _color;



	vertices[4].position = XMFLOAT3(-0.5f, -0.5f, -0.5f);
	vertices[4].color = _color;

	vertices[5].position = XMFLOAT3(-0.5f, 0.5f, -0.5f);
	vertices[5].color = _color;

	vertices[6].position = XMFLOAT3(0.5f, 0.5f, -0.5f);
	vertices[6].color = _color;

	vertices[7].position = XMFLOAT3(0.5f, -0.5f, -0.5f);
	vertices[7].color = _color;

	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 2;



	indices[6] = 6;
	indices[7] = 2;
	indices[8] = 3;
	indices[9] = 3;
	indices[10] = 7;
	indices[11] = 6;



	indices[12] = 5;
	indices[13] = 6;
	indices[14] = 7;
	indices[15] = 7;
	indices[16] = 4;
	indices[17] = 5;



	indices[18] = 1;
	indices[19] = 5;
	indices[20] = 4;
	indices[21] = 4;
	indices[22] = 0;
	indices[23] = 1;



	indices[24] = 6;
	indices[25] = 5;
	indices[26] = 1;
	indices[27] = 1;
	indices[28] = 2;
	indices[29] = 6;


	indices[30] = 3;
	indices[31] = 0;
	indices[32] = 4;
	indices[33] = 4;
	indices[34] = 7;
	indices[35] = 3;
#pragma endregion

	bool result = initializeBuffers(device, vertices, indices, 8, 36);

	delete[] vertices;
	delete[] indices;

	return result;
}

void MCube::setColor(XMFLOAT4 color)
{
	_color = color;
}

