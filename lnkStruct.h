#pragma once
#include <WINDOWS.h>
#include <vector>
#include <stack>
#include <fstream>
#define HasLinkTargetIDList 0x1
#define HasLinkInfo 0x2
#define HasName 0x4
#define HasRelativePath 0x8
#define HasWorkingDir 0x10
#define HasArguments 0x20
#define HasIconLocation 0x30
#define IsUnicode 0x40

typedef struct _LNK_HEADER {
	DWORD headerSize;
	CLSID LinkCLSID;
	DWORD LinkFlags;
	DWORD FileAttributes;
	FILETIME CreationTime;
	FILETIME AccessTime;
	FILETIME WriteTime;
	DWORD FileSize;
	DWORD IconIndex;
	DWORD ShowCommand;
	WORD HotKey;
	WORD Reserved1;
	DWORD Reserved2;
	DWORD Reserved3;
}LNK_HEADER,*LPLNK_HEADER;

typedef struct _FAT_DATE {
	WORD date;
	WORD time;
}FAT_DATE;

class LnkObject {
protected:
public:
	WORD Size;
	virtual int Write(std::ofstream& ifs)=0;
};

class LinkTargetIDList :public LnkObject {
public:
	std::stack<LnkObject*> shellItem;
	const WORD TerminalID=0;
	int Write(std::ofstream& ifs) override;
	int add(LnkObject* shellItem);
};

class VoluteShellItem :public LnkObject {
public:
	BYTE Indicator;
	char* name;
	int Write(std::ofstream& ifs) override;
};
class Beef0004 :public LnkObject {
public:
	WORD exVirsion = 9;
	const DWORD signature = 0xbeef0004;
	FAT_DATE createTime;
	FAT_DATE lastAccess;
	WORD Virsion = 0x2e;
	const WORD padding = 0;
	const UINT64 NTFSfile = 0;
	const UINT64 padding2 = 0;
	WORD longStringSize;
	DWORD padding3 = 0;
	const DWORD unknown = 0;
	char16_t* longStringName;
	WORD offset;
	Beef0004(FAT_DATE c, FAT_DATE l, char16_t* n);
	int Write(std::ofstream& ifs) override;
};

class FileEntryItem :public LnkObject {
public:
	BYTE Indicator;
	BYTE Padding=0;
	DWORD FileSize;
	FAT_DATE lastModify;
	WORD atrribute;
	char* PrimaryName;
	Beef0004 ex04;
	FileEntryItem(BYTE I, DWORD F, FAT_DATE l, WORD a, char* P);
	int Write(std::ofstream& ifs) override;
};


