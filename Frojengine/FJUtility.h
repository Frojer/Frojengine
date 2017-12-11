#pragma once

#include <fstream>

wchar_t* FileNameExtension(const wchar_t* i_fileName, wchar_t* o_extension, int i_size);
std::wstring GetFilePath(std::wstring _fileName);
std::wstring GetFileName(std::wstring _fileName);