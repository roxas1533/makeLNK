#include "lnkStruct.h"

int LinkTargetIDList::Write(std::ofstream& ifs)
{
    Size += sizeof(Size) + sizeof(TerminalID);
    ifs.write((char*)&Size, sizeof(Size));
    for (; !shellItem.empty();) {
        (shellItem.top())->Write(ifs);
        shellItem.pop();
    }
    ifs.write((char*)&TerminalID, sizeof(TerminalID));
    return 0;
}

int LinkTargetIDList::add(LnkObject* shellItem)
{
    this->shellItem.push(shellItem);
    Size += shellItem->Size;
    return 0;
}

int VoluteShellItem::Write(std::ofstream& ifs)
{
    ifs.write((char*)&Size, sizeof(Size));
    ifs.write((char*)&Indicator, sizeof(Indicator));
    for(int i=0; *(name+i)!='\0'; i++)
        ifs.write((char*)(name + i), sizeof(char));
    return 0;
}

FileEntryItem::FileEntryItem(BYTE I, DWORD F, FAT_DATE l, WORD a, char* P):Indicator(I),
FileSize(F), lastModify(l), atrribute(a), PrimaryName(P), ex04(FAT_DATE{}, FAT_DATE{}, 0) {
    Size += sizeof(Indicator);
    Size += sizeof(Padding);
    Size += sizeof(FileSize);
    Size += sizeof(lastModify);
    Size += sizeof(atrribute);
    //Size += ;
    Size += ex04.Size;
}

int FileEntryItem::Write(std::ofstream& ifs)
{
    ifs.write((char*)&Size, sizeof(Size));
    ifs.write((char*)&Indicator, sizeof(Indicator));
    ifs.write((char*)&Padding, sizeof(Padding));
    ifs.write((char*)&lastModify, sizeof(lastModify));
    ifs.write((char*)&atrribute, sizeof(atrribute));
    for (int i = 0; *(PrimaryName + i) != '\0'; i++)
        ifs.write((char*)(PrimaryName + i), sizeof(char));
    ex04.Write(ifs);
    return 0;
}


Beef0004::Beef0004(FAT_DATE c, FAT_DATE l, char16_t* n) :createTime(c), lastAccess(l), longStringName(n) {}
int Beef0004::Write(std::ofstream& ifs)
{
    ifs.write((char*)&exVirsion, sizeof(exVirsion));
    ifs.write((char*)&signature, sizeof(signature));
    ifs.write((char*)&createTime, sizeof(createTime));
    ifs.write((char*)&lastAccess, sizeof(lastAccess));
    ifs.write((char*)&Virsion, sizeof(Virsion));
    ifs.write((char*)&padding, sizeof(padding));
    ifs.write((char*)&NTFSfile, sizeof(NTFSfile));
    ifs.write((char*)&padding2, sizeof(padding2));
    ifs.write((char*)&longStringSize, sizeof(longStringSize));
    ifs.write((char*)&padding3, sizeof(padding3));
    ifs.write((char*)&unknown, sizeof(unknown));
    for (int i = 0; *(longStringName + i) != u'\0'; i++)
        ifs.write((char*)(longStringName + i), sizeof(char16_t));
    ifs.write((char*)&offset, sizeof(offset));
    return 0;
}