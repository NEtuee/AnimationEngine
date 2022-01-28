#include "MQuad.h"
#include <iostream>

bool MQuad::initialize(ID3D11Device* device)
{
	if (_width <= 0 || _height <= 0)
		return false;

	int vertexCount = (_width + 1) * (_height + 1);
	int indexCount = _width * _height * 6;

	float halfWidth = (static_cast<float>(_width) * _rectSize) * 0.5f;
	float halfHeight = (static_cast<float>(_height) * _rectSize) * 0.5f;

	VertexType* vertices = new VertexType[vertexCount];
	if (vertices == nullptr)
	{
		return false;
	}

	unsigned long* indices = new unsigned long[indexCount];
	if (indices == nullptr)
	{
		return false;
	}

	for (float j = 0; j < _height + 1; ++j)
	{
		for (float i = 0; i < _width + 1; ++i)
		{
			int current = j * (_width + 1) + i;
			vertices[current].position = XMFLOAT3(
				i * _rectSize - halfWidth,
				0.f,
				j * _rectSize - halfHeight);
			vertices[current].color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		}

	}

	int drawCount = 0;
	for (int i = 0; i < indexCount;)
	{
		for (int j = 0; j < _width; ++j)
		{
			int index = j + drawCount;
			int upIndex = (index + (_width + 1));
			
			indices[i] = index;
			indices[i + 1] = upIndex;
			indices[i + 2] = upIndex + 1;

			indices[i + 3] = upIndex + 1;
			indices[i + 4] = index + 1;
			indices[i + 5] = index;

			i += 6;
		}
		
		drawCount += _width + 1;
	}

	bool result = initializeBuffers(device, vertices, indices, vertexCount, indexCount);

	delete[] vertices;
	delete[] indices;

	return result;
}

void MQuad::setValue(float size, int width, int height)
{
    _rectSize = size;
    _width = width;
    _height = height;
}
