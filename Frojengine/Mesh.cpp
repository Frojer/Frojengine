#include "Mesh.h"
#include "FJUtility.h"

unordered_map<UINT, CMesh*> CMesh::_meshMap;
LPDEVICE CMesh::_pDevice = nullptr;
LPDXDC CMesh::_pDXDC = nullptr;

CMesh::CMesh()
	: _pVB(nullptr), _pIB(nullptr)
{
	_meshMap.insert(pair<UINT, CMesh*>(GetID(), this));
}

CMesh::CMesh(vector<VF_PNT> i_verts, vector<IndexFormat> i_indics)
	: _pVB(nullptr), _pIB(nullptr), m_verts(i_verts), m_indics(i_indics)
{
	_meshMap.insert(pair<UINT, CMesh*>(GetID(), this));
}


CMesh::~CMesh()
{
	_meshMap.erase(GetID());

	SAFE_RELEASE(_pVB);
	SAFE_RELEASE(_pIB);

	m_verts.clear();
	m_indics.clear();
}


void CMesh::ClearMap()
{
	auto i = _meshMap.begin();

	while (i != _meshMap.end())
		delete (i++)->second;
}


CMesh* CMesh::Find(UINT id)
{
	if (_meshMap.find(id) == _meshMap.end())
		return nullptr;

	return _meshMap[id];
}


CMesh* CMesh::Find(wstring name)
{
	FOR_STL(_meshMap)
	{
		if (iter->second->m_name == name)
			return iter->second;
	}

	return nullptr;
}


MATRIXA CMesh::GetWorldMatrix(const VECTOR3& pos, const VECTOR3& rot, const VECTOR3& scale)
{
	MATRIXA mPos, mRot, mScale;
	MATRIXA mWorld;

	mPos = DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&pos));
	mRot = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rot));
	mScale = DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&scale));

	return mScale * mRot * mPos;
}


void CMesh::Render()
{
	//----------------------------------------------------------------------
	// 바른 렌더링 결과를 위해서는 아래의 조건이 동일 또는 호환되어야 합니다.★
	// 1.정점 버퍼의 데이터.  Vertex Buffer Data
	// 2.정점 구조 Vertex Format (Input Layout)
	// 3.셰이더 함수의 입력구조.  Vertex Shader (Input Layout)
	// 4.각종 변환 처리 Vertex Transform
	//----------------------------------------------------------------------
	//정점 버퍼 설정. Set vertex buffer
	//디바이스의 입력-조립 스테이지 (Input-Assembler Stage) 에 정점버퍼를 연결.(Binding)
	UINT stride = sizeof(VF_PNT);
	UINT offset = 0;
	_pDXDC->IASetVertexBuffers(0, 1, &_pVB, &stride, &offset);			// 버텍스 버퍼 세팅
	_pDXDC->IASetIndexBuffer(_pIB, DXGI_FORMAT_R32_UINT, offset);		// 인덱스 버퍼 세팅

	// 기하 위상 구조 설정 Set primitive topology
	_pDXDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool CMesh::CreateBuffer(vector<VF_PNT>& i_vertics, vector<IndexFormat>& i_indics)
{
	m_verts = i_vertics;
	m_indics = i_indics;

	//---------------------------
	// 정점 버퍼 Vertex Buffer 생성
	//---------------------------
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;					// 버퍼 사용방식
	bd.ByteWidth = m_verts.size() * sizeof(VF_PNT);	// 버퍼 크기
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// 버퍼 용도 : "정점 버퍼" 용로 설정 
	bd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA rd;				
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = &m_verts[0];						// 버퍼에 들어갈 데이터 설정 : "정점들"..
	
	//정점 버퍼 생성.
	hr = _pDevice->CreateBuffer(&bd, &rd, &_pVB);
	if (FAILED(hr))
		return false;



	//---------------------------
	// 인덱스 버퍼 Index Buffer 생성
	//---------------------------
	hr = S_OK;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;					// 버퍼 사용방식
	bd.ByteWidth = m_indics.size() * sizeof(IndexFormat);	// 버퍼 크기
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;			// 버퍼 용도 : "인덱스 버퍼" 용로 설정 
	bd.CPUAccessFlags = 0;

	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = &m_indics[0];					//버퍼에 들어갈 데이터 설정 : "정점들"..

	//정점 버퍼 생성.
	hr = _pDevice->CreateBuffer(&bd, &rd, &_pIB);
	if (FAILED(hr))
		return false;

	return true;
}