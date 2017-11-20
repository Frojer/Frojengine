#include "Debug.h"

LPDEVICE Debug::_pDevice = nullptr;
LPDXDC Debug::_pDXDC = nullptr;
LPVERTEXBUFFER Debug::_pVB = nullptr;
VECTOR3 Debug::_linePos[2];
CMaterial* Debug::_pLineMtrl = nullptr;

void Debug::DrawLine(VECTOR3 start, VECTOR3 end, COLOR col)
{
	HRESULT hr = S_OK;

	if (_linePos[0].x == start.x &&
		_linePos[0].y == start.y &&
		_linePos[0].z == start.z &&
		_linePos[1].x == start.x &&
		_linePos[2].y == start.y &&
		_linePos[3].z == start.z)
	//if (_linePos[0] == start && _linePos[1] == end)
		return;

	_linePos[0] = start;
	_linePos[1] = end;

	if (_pVB == nullptr)
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(_linePos);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA rd;
		ZeroMemory(&rd, sizeof(rd));
		rd.pSysMem = _linePos;

		hr = _pDevice->CreateBuffer(&bd, &rd, &_pVB);

		if (FAILED(hr))
			return;
	}

	D3D11_MAPPED_SUBRESOURCE mr;
	ZeroMemory(&mr, sizeof(mr));

	//상수버퍼 접근
	hr = _pDXDC->Map(_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);

	//상수 버퍼 갱신.
	memcpy(mr.pData, _linePos, sizeof(_linePos));

	//상수버퍼 닫기.
	_pDXDC->Unmap(_pVB, 0);

	UINT stride = sizeof(VECTOR3);
	UINT offset = 0;
	_pDXDC->IASetVertexBuffers(0, 1, &_pVB, &stride, &offset);

	// 기하 위상 구조 설정 Set primitive topology
	_pDXDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	
	if (_pLineMtrl == nullptr)
	{
		_pLineMtrl = new CMaterial(CShader::Find(L"Line"));
	}

	_pLineMtrl->SetVector(0, col);
	_pLineMtrl->UpdateConstantBuffer(XMMatrixIdentity());
	_pLineMtrl->Render();

	_pDXDC->Draw(2, 0);
}