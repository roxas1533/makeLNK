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
	std::ofstream writeLnk;
	writeLnk.open(LnkPath, std::ios::out | std::ios::binary | std::ios::trunc);
	writeLnk.write((char*)&(newLnk.lnkHeader), sizeof(LNK_HEADER));
	if (linkflag & HasLinkTargetIDList) {
		LinkTargetIDList lti;
		std::filesystem::path pathParse(path);
		for (;;) {
			GetFileAttributesExA(pathParse.string().c_str(), GetFileExInfoStandard, &fileData);
			std::string aa = pathParse.filename().string();
			aa += '\0';
			//TODO TimetoFat�̍쐬

			FileEntryItem* FEI = new FileEntryItem((BYTE)(pathParse.has_extension() ? 0x32 : 0x31), fileData.nFileSizeLow, fileData,(WORD)fileData.dwFileAttributes,aa);
			

			pathParse = pathParse.parent_path();
			lti.add(FEI);

			if (pathParse.root_path() == pathParse) {
				VoluteShellItem* vls = new VoluteShellItem(0x2F,"C:\\");
				lti.add(vls);
				break;
			}
		}
		lti.add(new RootFolderShellItem());
		lti.Write(writeLnk);
		if (linkflag & HasLinkInfo) {
			LinkInfo lnkinfo(path);
			lnkinfo.Write(writeLnk);
		}
	}
	DWORD terminalBlockEx = 0;
	writeLnk.write((char*)&(terminalBlockEx), sizeof(DWORD));
	writeLnk.close();
}