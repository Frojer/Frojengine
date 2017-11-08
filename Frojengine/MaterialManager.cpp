#include "MaterialManager.h"

unordered_map<LPCWSTR, CMaterial*> MaterialManager::_mtrlMap;

bool MaterialManager::InsertMaterial(CMaterial* i_pMtrl)
{
	if (i_pMtrl == nullptr)
		return false;

	//_mtrlMap.insert(pair<LPCWSTR, CMaterial*>(i_pMtrl->m_Name, i_pMtrl));
	return true;
}


CMaterial* MaterialManager::GetMaterial(LPCWSTR i_fileName)
{
	if (_mtrlMap.find(i_fileName) == _mtrlMap.end())
		return nullptr;

	return _mtrlMap[i_fileName];
}


void MaterialManager::Clear()
{
	auto i = _mtrlMap.begin();

	while (i != _mtrlMap.end())
	{
		delete i->second;
		i->second = nullptr;
		_mtrlMap.erase(i++);
	}
}


void MaterialManager::DeleteMaterial(LPCWSTR i_fileName)
{
	if (_mtrlMap.find(i_fileName) != _mtrlMap.end())
	{
		delete _mtrlMap[i_fileName];
		_mtrlMap[i_fileName] = nullptr;

		_mtrlMap.erase(i_fileName);
	}
}