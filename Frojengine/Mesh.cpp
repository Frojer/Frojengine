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
	UINT stride = sizeof(VF_PNT);
	UINT offset = 0;
	_pDXDC->IASetVertexBuffers(0, 1, &_pVB, &stride, &offset);			// ���ؽ� ���� ����
	_pDXDC->IASetIndexBuffer(_pIB, DXGI_FORMAT_R32_UINT, offset);		// �ε��� ���� ����

	// ���� ���� ���� ���� Set primitive topology
	_pDXDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool CMesh::CreateBuffer(vector<VF_PNT>& i_vertics, vector<IndexFormat>& i_indics)
{
	m_verts = i_vertics;
	m_indics = i_indics;

	//---------------------------
	// ���� ���� Vertex Buffer ����
	//---------------------------
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;					// ���� �����
	bd.ByteWidth = m_verts.size() * sizeof(VF_PNT);	// ���� ũ��
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// ���� �뵵 : "���� ����" ��� ���� 
	bd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA rd;				
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = &m_verts[0];						// ���ۿ� �� ������ ���� : "������"..
	
	//���� ���� ����.
	hr = _pDevice->CreateBuffer(&bd, &rd, &_pVB);
	if (FAILED(hr))
		return false;



	//---------------------------
	// �ε��� ���� Index Buffer ����
	//---------------------------
	hr = S_OK;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;					// ���� �����
	bd.ByteWidth = m_indics.size() * sizeof(IndexFormat);	// ���� ũ��
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;			// ���� �뵵 : "�ε��� ����" ��� ���� 
	bd.CPUAccessFlags = 0;

	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = &m_indics[0];					//���ۿ� �� ������ ���� : "������"..

	//���� ���� ����.
	hr = _pDevice->CreateBuffer(&bd, &rd, &_pIB);
	if (FAILED(hr))
		return false;

	return true;
}