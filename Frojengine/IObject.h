#pragma once

#include <list>
#include "FJDefine.h"

using namespace std;

class IObject
{
private:
	static unsigned int _countID;
	unsigned int  _ID;
	bool _isData;

	static list<IObject*> _objList;
	static list<IObject*> _dataObjList;

public:
	wstring m_name;

private:
	IObject(const IObject& obj) { }
	IObject(bool isData) { _isData = isData; _ID = _countID++; _dataObjList.push_back(this); }
	void SetID(unsigned int id) { _ID = id; }

public:
	IObject() { _isData = false; _ID = _countID++; _objList.push_back(this); }
	virtual ~IObject()
	{
		FOR_STL(_objList)
		{
			if ((*iter) == this)
			{
				_objList.erase(iter);
				break;
			}
		}

		FOR_STL(_dataObjList)
		{
			if ((*iter) == this)
			{
				_dataObjList.erase(iter);
				break;
			}
		}
	}

	unsigned int GetID() { return _ID; }

	static IObject* FindObject(unsigned int id)
	{
		FOR_STL(_objList)
		{
			if ((*iter)->GetID() == id)
				return (*iter);
		}

		return nullptr;
	}

	// CObject* CObject::CopyObject(const CObject* origin)
	friend class CObject;
};