#pragma once

#include "Frojengine.h"
#include <fstream>

class CObject;

class FileLoader
{
private:
	static wstring _fileName;
private:
	static CObject* LoadObjFile(LPCWSTR i_fileName);
	static CObject* LoadXFile(LPCWSTR i_fileName);
	static CObject* XFileLoadFrame(CObject* pObj, wifstream& file);

public:
	static CObject* ObjectFileLoad(LPCWSTR i_fileName);
};