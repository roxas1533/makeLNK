#pragma once
#include "lnkStruct.h"
#include "combaseapi.h"
#include <string>
#include <iostream>
class LnkClass{
public:
	LNK_HEADER lnkHeader;
};

void makeLNK(DWORD fileAttribute, std::wstring path, std::wstring LnkPath);