#include "MeshManager.h"

MeshManager::MeshManager()
{

}


MeshManager::~MeshManager()
{

}


bool MeshManager::InsertMesh(LPCWSTR i_fileName)
{
	bool result = true;
	CMesh* pMesh;
	
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

			return false;
		}

		result = pMesh->LoadObjFile(i_fileName);

		if (!result)
		{
			delete pMesh;
			pMesh = nullptr;

			ErrMsgBox(L"Failed to LoadObjFile()");

			return false;
		}
	}

	return true;
}


bool MeshManager::GetMesh(LPCWSTR i_fileName)
{
	bool result = true;

	if (_meshMap.find(i_fileName) == _meshMap.end())
	{
		result = InsertMesh(i_fileName);
	}

	return result;
}