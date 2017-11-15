#include "FJUtility.h"

#include <wchar.h>

wchar_t* FileNameExtension(const wchar_t* i_fileName, wchar_t* o_extension, int i_size)
{
	int count = 0;
	int comma = 0;

	while (i_fileName[count++] != '\0') {}
	comma = --count;
	while (i_fileName[--comma] != '.') {}

	wcsncpy_s(o_extension, i_size, &i_fileName[comma], count - comma + 1 >= i_size ? i_size : count - comma + 1);

	return o_extension;
}