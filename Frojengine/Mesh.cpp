#include "Mesh.h"

CMesh::CMesh()
{

}

CMesh::CMesh(LPVERTEXBUFFER i_pVB, LPINDEXBUFFER i_pIB, vector<FVF_PNT> i_verts, vector<unsigned int> i_indics)
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
	
}

bool CMesh::LoadXFile(LPCWSTR i_fileName)
{

}