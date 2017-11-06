#include "Mesh.h"

CMesh::CMesh()
{

}

CMesh::CMesh(LPVERTEXBUFFER i_pVB, LPINDEXBUFFER i_pIB, vector<VF_PNT> i_verts, vector<unsigned int> i_indics)
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

	while (i_fileName[count++] == '\0') {}
	comma = --count;
	while (i_fileName[--comma] == '.') {}

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