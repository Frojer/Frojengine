#include "Debug.h"
#include "FJMath.h"

LPVERTEXBUFFER Debug::_pVB = nullptr;
VECTOR Debug::_linePos[2];
CMaterial* Debug::_pLineMtrl = nullptr;

void Debug::DrawLine(VECTOR3 start, VECTOR3 end, COLOR col)
{
#ifdef _DEBUG
	DrawLine(XMLoadFloat3(&start), XMLoadFloat3(&end), col);
#endif
}


void Debug::DrawLine(VECTOR start, VECTOR end, COLOR col)
{
#ifdef _DEBUG
	HRESULT hr = S_OK;
	VECTOR equal1 = XMVectorEqual(_linePos[0], start);
	VECTOR equal2 = XMVectorEqual(_linePos[1], end);

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


	if (!(equal1.m128_f32[0] &&
		equal1.m128_f32[1] &&
		equal1.m128_f32[2] &&
		equal1.m128_f32[3] &&
		equal2.m128_f32[0] &&
		equal2.m128_f32[1] &&
		equal2.m128_f32[2] &&
		equal2.m128_f32[3]))
	{
		_linePos[0] = start;
		_linePos[1] = end;
		
		D3D11_MAPPED_SUBRESOURCE mr;
		ZeroMemory(&mr, sizeof(mr));

		//상수버퍼 접근
		hr = _pDXDC->Map(_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);

		//상수 버퍼 갱신.
		memcpy(mr.pData, _linePos, sizeof(_linePos));

		//상수버퍼 닫기.
		_pDXDC->Unmap(_pVB, 0);
	}

	UINT stride = sizeof(VECTOR);
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

	FJRenderingEngine::SetRSState(0);
	FJRenderingEngine::SetDSState(0, 0);
	FJRenderingEngine::SetBlendState(BS_ADD);

	_pDXDC->Draw(2, 0);
#endif
}


void Debug::DrawNormal(const CObject* obj, COLOR col)
{
#ifdef _DEBUG
	vector<VF_PNT> list = obj->m_pRenderer->m_pMesh->m_verts;

	MATRIXA mWorld = obj->m_pTransform->GetWorldMatrix();
	VECTOR start, end;

	for (UINT i = 0; i < list.size(); i++)
	{
		start = XMVector3TransformCoord(XMLoadFloat3(&list[i].pos), mWorld);
		end = XMVector3TransformCoord(XMLoadFloat3(&(list[i].pos + list[i].nor)), mWorld);

		Debug::DrawLine(start, end, col);
	}
#endif
}