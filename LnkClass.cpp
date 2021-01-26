#include "LnkClass.h"
#include <iostream>
#include <fstream>
#include <filesystem>
void makeLNK(DWORD linkflag, DWORD fileAttribute, std::string path,std::string LnkPath) {
	WIN32_FILE_ATTRIBUTE_DATA fileData;
	std::cout << path<<"\n";
	if (!GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &fileData))
		return;



	LnkClass newLnk;
	newLnk.lnkHeader.headerSize = 0x0000004C;
	HRESULT r;
	r = CLSIDFromString((LPCOLESTR)L"{00021401-0000-0000-C000-000000000046}", &(newLnk.lnkHeader.LinkCLSID));
	newLnk.lnkHeader.LinkFlags = linkflag;
	newLnk.lnkHeader.FileAttributes = fileAttribute;
	newLnk.lnkHeader.CreationTime = fileData.ftCreationTime;
	newLnk.lnkHeader.AccessTime = fileData.ftLastAccessTime;
	newLnk.lnkHeader.WriteTime = fileData.ftLastWriteTime;
	newLnk.lnkHeader.FileSize = fileData.nFileSizeLow;
	newLnk.lnkHeader.IconIndex = 0;
	newLnk.lnkHeader.ShowCommand = 1;
	newLnk.lnkHeader.HotKey = 0;
	newLnk.lnkHeader.Reserved1 = 0;
	newLnk.lnkHeader.Reserved2 = 0;
	newLnk.lnkHeader.Reserved3 = 0;

	if (linkflag & HasLinkTargetIDList) {
		std::filesystem::path pathParse(path);
		for (;;) {
			if (GetFileAttributesExA(pathParse.string().c_str(), GetFileExInfoStandard, &fileData))
				std::cout << fileData.nFileSizeLow << "\n";
			std::string aa = pathParse.filename().string();
			aa += '\0';
			//TODO TimetoFat�̍쐬
			FileEntryItem* FEI = new FileEntryItem(pathParse.has_extension() ? 0x32 : 0x31, fileData.nFileSizeLow,aa.c_str());

			pathParse = pathParse.parent_path();
			if (pathParse.root_path() == pathParse)
				break;
		}
		//LinkTargetIDList lti;

		//lti.add()
	}

	std::ofstream writeLnk;
	writeLnk.open(LnkPath, std::ios::out | std::ios::binary | std::ios::trunc);
	writeLnk.write((char*)&(newLnk.lnkHeader), sizeof(LNK_HEADER));
	writeLnk.close();
}