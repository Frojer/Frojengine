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
	
	// ���� ������ ���� ���� �迭 
	// �Ϲ������δ� �ܺ�(HDD) ���� �ε��մϴ�.
	VF_Pos	Vertices[] = {
		//���� ��ǥ�� Ŭ������ (Clip Space) �������� ��ġ�մϴ�.�� 
		// ��ǥ (x, y, z) 
		{ XMFLOAT3(-0.5f, -0.5f, 0.0f) },	//v0, ����0��.
		{ XMFLOAT3(0.0f,  0.5f, 0.0f) },	//v1, ����1��. 
		{ XMFLOAT3(0.5f, -0.5f, 0.0f) },	//v2, ����2��.
	};

	m_verts.push_back(Vertices[0]);
	m_verts.push_back(Vertices[1]);
	m_verts.push_back(Vertices[2]);
	

	// ���� ���� Vertex Buffer ���� ��
	// DX10 ���� �޸�(����) �� �԰��� ���յǾ����ϴ�.  
	// ���� ���� ������ �� �뵵(VB, IB..)�� �����ؾ� �մϴ�. 
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;						// ���� �����
	bd.ByteWidth = m_verts.size() * sizeof(VF_Pos);		// ���� ũ��;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// ���� �뵵 : "���� ����" ��� ���� 
	bd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA InitData;				
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &m_verts[0];						// ���ۿ� �� ������ ���� : "������"..
	
	//���� ���� ����.
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
	// �ٸ� ������ ����� ���ؼ��� �Ʒ��� ������ ���� �Ǵ� ȣȯ�Ǿ�� �մϴ�.��
	// 1.���� ������ ������.  Vertex Buffer Data
	// 2.���� ���� Vertex Format (Input Layout)
	// 3.���̴� �Լ��� �Է±���.  Vertex Shader (Input Layout)
	// 4.���� ��ȯ ó�� Vertex Transform
	//----------------------------------------------------------------------
	//���� ���� ����. Set vertex buffer
	//����̽��� �Է�-���� �������� (Input-Assembler Stage) �� �������۸� ����.(Binding)
	UINT stride = sizeof(VF_Pos);
	UINT offset = 0;
	_pDXDC->IASetVertexBuffers(0, 1, &_pVB, &stride, &offset);

	// ���� ���� ���� ���� Set primitive topology
	_pDXDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}