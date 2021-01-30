#pragma once
#include "lnkStruct.h"
#include "combaseapi.h"
#include <string>
#include <iostream>
class LnkClass{
public:
	LNK_HEADER lnkHeader;
};

void makeLNK(DWORD linkflag, DWORD fileAttribute, std::string path, std::string LnkPath);