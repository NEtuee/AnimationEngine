#pragma once
#include "DirectXDefine.h"

struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

class Model : public AlignedAllocationPolicy<16>
{
public:
					Model();
					~Model();

	virtual bool	initialize(ID3D11Device* device);
	void			destroyModel();
	void			render(ID3D11DeviceContext* deviceContext);
	void			setRenderLine(bool value) { _renderLine = value; }
	int				getIndexCount();
protected:
	bool			initializeBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices,
						int vertexCount, int indexCount);
private:
	void			destroyBuffers();
	void			renderBuffers(ID3D11DeviceContext* deviceContext);
private:
	ID3D11Buffer*	_vertexBuffer = nullptr;
	ID3D11Buffer*	_indexBuffer = nullptr;
	int				_vertexCount = 0;
	int				_indexCount = 0;

	bool			_renderLine = false;

};

