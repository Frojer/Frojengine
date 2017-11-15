#pragma once

#include <list>
#include "FJDefine.h"

using namespace std;

class IObject
{
private:
	static unsigned int _countID;
	unsigned int  _ID;

	static list<IObject*> _objList;

public:
	wstring m_name;

public:
	IObject() { _ID = _countID++; _objList.push_back(this); }
	~IObject() { FOR_STL(_objList) { if ((*iter) == this) _objList.erase(iter); break; } }

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
};