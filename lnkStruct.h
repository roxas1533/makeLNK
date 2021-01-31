#pragma once
#include <WINDOWS.h>
#include <vector>
#include <stack>
#include <fstream>
#include <iostream>
#define HasLinkTargetIDList 0x1
#define HasLinkInfo 0x2
#define HasName 0x4
#define HasRelativePath 0x8
#define HasWorkingDir 0x10
#define HasArguments 0x20
#define HasIconLocation 0x40
#define IsUnicode 0x80

#define FILE_ATTRIBUTE_READONLY 0x1
#define FILE_ATTRIBUTE_HIDDEN 0x2
#define FILE_ATTRIBUTE_SYSTEM 0x4
#define FILE_ATTRIBUTE_DIRECTORY 0x10
#define HasArguments 0x20
#define FILE_ATTRIBUTE_ARCHIVE 0x40


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
	WORD Size=0;
	DWORD DSize=0;
	virtual int Write(std::ofstream& ifs)=0;
};

class LinkTargetIDList :public LnkObject {
public:
	std::stack<LnkObject*> shellItem;
	const WORD TerminalID=0;
	int Write(std::ofstream& ifs) override;
	int add(LnkObject* shellItem);
};

class RootFolderShellItem :public LnkObject {
public:
	RootFolderShellItem();
	BYTE Indicator=0x1F;
	BYTE SortIndex=0x50;
	GUID ShellFolder;
	int Write(std::ofstream& ifs) override;
};

class VoluteShellItem :public LnkObject {
public:
	VoluteShellItem(BYTE Indicator,const char* name);
	BYTE Indicator;
	const char* name;
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
	std::string longStringName;
	WORD offset;
	int Write(std::ofstream& ifs) override;
	Beef0004(FAT_DATE c, FAT_DATE l, std::string n);

};

class FileEntryItem :public LnkObject {
public:
	BYTE Indicator;
	const BYTE Padding;
	DWORD FileSize;
	FAT_DATE lastModify;
	WORD atrribute;
	std::string PrimaryName;
	FileEntryItem(BYTE I, DWORD F, WIN32_FILE_ATTRIBUTE_DATA l, WORD a, std::string P);
	Beef0004 ex04;

	int Write(std::ofstream& ifs) override;
};
FAT_DATE TimeToFat(FILETIME& time);


class VolumeID :public LnkObject {
public:

	DWORD DriveType;
	DWORD DriveSerialNumber;
	DWORD VolumeLabelOffset;
	DWORD VolumeLabelOffsetUnicode=0;
	std::string data;
	int Write(std::ofstream& ifs) override;
	VolumeID();
};

class LinkInfo :public LnkObject {
public:
	const DWORD LinkInfoHeaderSize = 0;
	DWORD LinkInfoFlags;
	DWORD VolumeIDOffset;
	DWORD LocalBasePathOffset;
	DWORD CommonNetworkRelativeLinkOffset;
	DWORD CommonPathSuffixOffset;
	DWORD LocalBasePathOffsetUnicode;
	DWORD CommonPathSuffixOffsetUnicode;
	VolumeID vol;
	std::string LocalBasePath;
	char CommonPathSuffix = '\0';
	LinkInfo(std::string path);
	int Write(std::ofstream& ifs) override;
};

class StringData :public LnkObject {
public:
	WORD CountCharacters;
	std::u16string str;
	StringData(std::u16string str);
	int Write(std::ofstream& ifs) override;
};