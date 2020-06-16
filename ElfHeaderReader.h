#ifndef ELFHEADERREADER_H
#define ELFHEADERREADER_H

#include "HeaderReader.h"

#include <elf.h>

#include <iostream> // makes size_t available
//#include <cstdint> // makes uint64_t available

// Standard types from elf manpage:
using       Elf64_Addr       = uint64_t;
using       Elf32_Addr       = uint32_t;
using       Elf64_Off        = uint64_t;
using       Elf32_Off        = uint32_t;
using       Elf64_Section    = uint16_t;
using       Elf64_Versym     = uint16_t;
using       Elf32_Section    = uint16_t;
using       Elf32_Versym     = uint16_t;
using       Elf_Byte         = uint8_t;
using       Elf64_Half       = uint16_t;
using       Elf64_Sword      = int32_t;
using       Elf64_Word       = uint32_t;
using       Elf64_Sxword     = int64_t;
using       Elf64_Xword      = uint64_t;
using       Elf32_Half       = uint16_t;
using       Elf32_Sword      = int32_t;
using       Elf32_Word       = uint32_t;
using       Elf32_Sxword     = int64_t;
using       Elf32_Xword      = uint64_t;

#if 0
constexpr size_t EI_NIDENT = 16;
#endif
struct Elf64Header
{
    unsigned char e_ident[EI_NIDENT];
    uint16_t      e_type;
    uint16_t      e_machine;
    uint32_t      e_version;
    Elf64_Addr    e_entry;
    Elf64_Off     e_phoff;
    Elf64_Off     e_shoff;
    uint32_t      e_flags;
    uint16_t      e_ehsize;
    uint16_t      e_phentsize;
    uint16_t      e_phnum;
    uint16_t      e_shentsize;
    uint16_t      e_shnum;
    uint16_t      e_shstrndx;
};

#if 0
constexpr uint32_t      ET_NONE                 = 0;
constexpr uint32_t      ET_REL                  = 1;
constexpr uint32_t      ET_EXEC                 = 2;
constexpr uint32_t      ET_DYN                  = 3;
constexpr uint32_t      ET_CORE                 = 4;
#endif

constexpr size_t ElfHeaderRealSize = 64;
void ElfHeaderExtract(HeaderParser &hp, Elf64Header &hdr, size_t size);
class ElfHeaderReader : public HeaderReader
{
public:
    ElfHeaderReader(HeaderParser::EndianNess eness);
    virtual ~ElfHeaderReader();
    void getHdr(Elf64Header &h);
private:
    virtual ssize_t getSize();
    virtual void extractHdr();
    Elf64Header h_;
};


struct Elf64Shdr
{
    uint32_t   sh_name;
    uint32_t   sh_type;
    uint64_t   sh_flags;
    Elf64_Addr sh_addr;
    Elf64_Off  sh_offset;
    uint64_t   sh_size;
    uint32_t   sh_link;
    uint32_t   sh_info;
    uint64_t   sh_addralign;
    uint64_t   sh_entsize;
};

constexpr size_t Elf64ShdrRealSize = 64;
void Elf64ShdrExtract(HeaderParser &hp, Elf64Shdr &hdr, size_t size);
class Elf64ShdrReader : public HeaderReader
{
public:
    Elf64ShdrReader(HeaderParser::EndianNess eness);
    virtual ~Elf64ShdrReader();
    void getHdr(Elf64Shdr &h);
private:
    virtual ssize_t getSize();
    virtual void extractHdr();
    Elf64Shdr h_;
};


class ElfSection final
{
public:
    ElfSection();
    ~ElfSection();
    void reset();
    void reserve(size_t alloc_sz);
    void append(std::istream &is, uint64_t pos, uint64_t size);
    uint8_t const *data() const;
    size_t size() const;
private:
    std::vector<uint8_t> data_;
};

class ElfStringTable final
{
public:
    ElfStringTable();
    ~ElfStringTable();
    void buildFromElfSection(ElfSection const &es);
    size_t getNumStrings() const;
    std::string getString(size_t n) const;
private:
    std::vector<std::string> str_vec_;
};

#endif // ELFHEADERREADER_H
