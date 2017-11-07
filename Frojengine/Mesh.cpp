#include "Mesh.h"

LPDEVICE CMesh::_pDevice = nullptr;
LPDXDC CMesh::_pDXDC = nullptr;

CMesh::CMesh()
	: _pVB(nullptr), _pIB(nullptr)
{

}

CMesh::CMesh(LPVERTEXBUFFER i_pVB, LPINDEXBUFFER i_pIB, vector<VF_Pos> i_verts, vector<unsigned int> i_indics)
	: _pVB(i_pVB), _pIB(i_pIB), m_verts(i_verts), m_indics(i_indics)
{

}


CMesh::~CMesh()
{
	SAFE_RELEASE(_pVB);
	SAFE_RELEASE(_pIB);

	m_verts.clear();
	m_indics.clear();
}


bool CMesh::LoadObjFile(LPCWSTR i_fileName)
{
	HRESULT hr = S_OK;
	
	// 기하 도형을 위한 정점 배열 
	// 일반적으로는 외부(HDD) 에서 로딩합니다.
	VF_Pos	Vertices[] = {
		//정점 좌표는 클립공간 (Clip Space) 기준으로 배치합니다.★ 
		// 좌표 (x, y, z) 
		{ XMFLOAT3(-0.5f, -0.5f, 0.0f) },	//v0, 정점0번.
		{ XMFLOAT3(0.0f,  0.5f, 0.0f) },	//v1, 정점1번. 
		{ XMFLOAT3(0.5f, -0.5f, 0.0f) },	//v2, 정점2번.
	};

	m_verts.push_back(Vertices[0]);
	m_verts.push_back(Vertices[1]);
	m_verts.push_back(Vertices[2]);
	

	// 정점 버퍼 Vertex Buffer 생성 ★
	// DX10 부터 메모리(버퍼) 의 규격이 통합되었습니다.  
	// 따라서 버퍼 생성시 그 용도(VB, IB..)를 결정해야 합니다. 
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;						// 버퍼 사용방식
	bd.ByteWidth = m_verts.size() * sizeof(VF_Pos);		// 버퍼 크기;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// 버퍼 용도 : "정점 버퍼" 용로 설정 
	bd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA InitData;				
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &m_verts[0];						// 버퍼에 들어갈 데이터 설정 : "정점들"..
	
	//정점 버퍼 생성.
	hr = _pDevice->CreateBuffer(&bd, &InitData, &_pVB);
	if (FAILED(hr))	return hr;

	return true;
}

bool CMesh::LoadXFile(LPCWSTR i_fileName)
{
	return true;
}

	
CMesh* CMesh::CreateMesh(LPCWSTR i_fileName)
{
	bool result = true;
	CMesh* pMesh = nullptr;

	int count = 0;
	int comma = 0;

	while (i_fileName[count++] != '\0') {}
	comma = --count;
	while (i_fileName[--comma] != '.') {}

	if (lstrcmp(&i_fileName[comma], L".obj") == 0)
	{
		pMesh = new CMesh;

		if (pMesh == nullptr)
		{
			ErrMsgBox(L"Failed to Memory Allocate.");

			return nullptr;
		}

		result = pMesh->LoadObjFile(i_fileName);

		if (!result)
		{
			delete pMesh;
			pMesh = nullptr;

			ErrMsgBox(L"Failed to LoadObjFile()");

			return nullptr;
		}
	}

	else if (lstrcmp(&i_fileName[comma], L".x") == 0)
	{
		pMesh = new CMesh;

		if (pMesh == nullptr)
		{
			ErrMsgBox(L"Failed to Memory Allocate.");

			return nullptr;
		}

		result = pMesh->LoadXFile(i_fileName);

		if (!result)
		{
			delete pMesh;
			pMesh = nullptr;

			ErrMsgBox(L"Failed to LoadXFile()");

			return nullptr;
		}
	}

	return pMesh;
}


void CMesh::UpdateBuffer(const VECTOR3& pos, const VECTOR3& rot, const VECTOR3& scale)
{
	MATRIXA mPos, mRot, mScale;
	MATRIXA mWorld, mView, mProj;

	mPos = DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&pos));
	mRot = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rot));
	mScale = DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&scale));

	//mWorld = mScale * mRot * mPos;
	//mView = ;
	//mProj = XMMatrixPerspectiveFovLH();

	
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
	UINT stride = sizeof(VF_Pos);
	UINT offset = 0;
	_pDXDC->IASetVertexBuffers(0, 1, &_pVB, &stride, &offset);

	// 기하 위상 구조 설정 Set primitive topology
	_pDXDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}