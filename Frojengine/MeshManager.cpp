#include "MeshManager.h"

unordered_map<LPCWSTR, CMesh*> MeshManager::_meshMap;

MeshManager::MeshManager()
{

}


MeshManager::~MeshManager()
{
	Clear();
}


bool MeshManager::InsertMesh(LPCWSTR i_fileName)
{
	CMesh* pMesh;
	
	if (_meshMap.find(i_fileName) != _meshMap.end())
		return false;

	pMesh = CMesh::CreateMesh(i_fileName);
	
	if (pMesh == nullptr)
		return false;

	_meshMap.insert(pair<LPCWSTR, CMesh*>(i_fileName, pMesh));

	return true;
}


CMesh* MeshManager::GetMesh(LPCWSTR i_fileName)
{
	bool result = true;

	if (_meshMap.find(i_fileName) == _meshMap.end())
	{
		result = InsertMesh(i_fileName);
	}

	if (!result)
		return nullptr;

	return _meshMap[i_fileName];
}


void MeshManager::DeleteMesh(LPCWSTR i_fileName)
{
	if (_meshMap.find(i_fileName) != _meshMap.end())
	{
		delete _meshMap[i_fileName];
		_meshMap[i_fileName] = nullptr;

		_meshMap.erase(i_fileName);
	}
}


void MeshManager::Clear()
{
	auto i = _meshMap.begin();

	while (i != _meshMap.end())
	{
		delete i->second;
		i->second = nullptr;
		_meshMap.erase(i++);
	}
}