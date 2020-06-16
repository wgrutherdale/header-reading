#include <iostream>
#include <fstream>

#include "HeaderReader.h"
#include "ElfHeaderReader.h"

void test00();
void test00Helper(std::string const &elf_name);
void verifyElfHeader(Elf64Header const &h);
void testSectionHeaders(std::ifstream &i_stream, Elf64Header &h);
void loadElfStringTable(std::ifstream &i_stream,  size_t pos, ElfStringTable &est);
void loadSectionHeader(std::ifstream &i_stream, uint64_t shdr_loc, Elf64Shdr &h);
void showElfStringTable(ElfStringTable &est);
std::string getShTypeStr(uint32_t tp);

int main()
{
    test00();
    return 0;
}


void test00()
{
    std::cout << "test00()\n";
    test00Helper("testb.o");
}

void test00Helper(std::string const &elf_name)
{
    std::cout << "  Reading file " << elf_name << "\n";
    std::ifstream i_stream;
    i_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    i_stream.open(elf_name, std::ios::in|std::ios::binary);
    Elf64Header h;
    ElfHeaderReader h_reader(HeaderParser::EndianLittle);
    h_reader.readHdr(i_stream);
    h_reader.getHdr(h);
    std::cout << "  e_ident ";
    for ( size_t i = 0;  i < EI_NIDENT;  ++i )
        std::cout << unsigned(h.e_ident[i]) << " ";
    std::cout << "\n";
    std::cout << "  e_type " << h.e_type << "\n";
    std::cout << "  e_machine " << h.e_machine << "\n";
    std::cout << "  e_version " << h.e_version << "\n";
    std::cout << "  e_entry " << h.e_entry << "\n";
    std::cout << "  e_phoff " << h.e_phoff << "\n";
    std::cout << "  e_shoff " << h.e_shoff << "\n";
    std::cout << "  e_flags " << h.e_flags << "\n";
    std::cout << "  e_ehsize " << h.e_ehsize << "\n";
    std::cout << "  e_phentsize " << h.e_phentsize << "\n";
    std::cout << "  e_phnum " << h.e_phnum << "\n";
    std::cout << "  e_shentsize " << h.e_shentsize << "\n";
    std::cout << "  e_shnum " << h.e_shnum << "\n";
    std::cout << "  e_shstrndx " << h.e_shstrndx << "\n";
    verifyElfHeader(h);
    testSectionHeaders(i_stream, h);
    i_stream.close();
}

void verifyElfHeader(Elf64Header const &h)
{
    if ( h.e_ident[EI_MAG0] != ELFMAG0 || h.e_ident[EI_MAG1] != ELFMAG1 ||
         h.e_ident[EI_MAG2] != ELFMAG2 || h.e_ident[EI_MAG3] != ELFMAG3 )
        throw std::logic_error("Elf Verif:  magic number failure");
    if ( h.e_ident[EI_CLASS] != ELFCLASS64 || h.e_ident[EI_DATA] != ELFDATA2LSB )
        throw std::logic_error("Elf Verif:  expecting 64 bit, little endian, got different");
}

void testSectionHeaders(std::ifstream &i_stream, Elf64Header &h)
{
    auto section_hdr_loc = h.e_shoff;
    auto num_sh_entries = h.e_shnum;
    auto sh_size = h.e_shentsize;
    auto shstrndx = h.e_shstrndx;
    std::cout << "shstrndx==" << shstrndx << "\n";
    ElfStringTable est;
    loadElfStringTable(i_stream, section_hdr_loc+sh_size*shstrndx, est);
    showElfStringTable(est);
    for ( decltype(num_sh_entries) i = 0;  i < num_sh_entries;  ++i )
    {
        uint64_t shdr_loc = section_hdr_loc + sh_size*i;
        std::cout << "reading section header " << i << " at " << shdr_loc << "\n";
        Elf64Shdr sh_hdr;
        loadSectionHeader(i_stream, shdr_loc, sh_hdr);
    }
}

void loadElfStringTable(std::ifstream &i_stream,  size_t pos, ElfStringTable &est)
{
    Elf64Shdr h;
    loadSectionHeader(i_stream, pos, h);
    size_t sec_pos = h.sh_offset;
    size_t sec_len = h.sh_size;
    ElfSection str_elf_section;
    str_elf_section.reserve(sec_len);
    str_elf_section.append(i_stream, sec_pos, sec_len);
    est.buildFromElfSection(str_elf_section);
}

void showElfStringTable(ElfStringTable &est)
{
    size_t n = est.getNumStrings();
    std::cout << "  Elf String Table (" << n << "):  ";
    for ( size_t i = 0;  i < n;  ++i )
    {
        std::cout << i << " --> " << est.getString(i) << " ";
    }
    std::cout << "\n";
}

void loadSectionHeader(std::ifstream &i_stream, uint64_t shdr_loc, Elf64Shdr &h)
{
    i_stream.seekg(shdr_loc);
    Elf64ShdrReader h_reader(HeaderParser::EndianLittle);
    h_reader.readHdr(i_stream);
    h_reader.getHdr(h);
    std::cout << "  " <<
                 "sh_name==" << h.sh_name <<
                 ", sh_type==" << h.sh_type << " (" << getShTypeStr(h.sh_type) << ")" <<
                 ", sh_flags==" << h.sh_flags <<
                 ", sh_addr==" << h.sh_addr <<
                 ", sh_offset==" << h.sh_offset <<
                 ", sh_size==" << h.sh_size <<
                 ", sh_link==" << h.sh_link <<
                 ", sh_info==" << h.sh_info <<
                 ", sh_addralign==" << h.sh_addralign <<
                 ", sh_entsize==" << h.sh_entsize <<
                 "\n";
}

std::string getShTypeStr(uint32_t tp)
{
    std::string tp_str("Unknown");
    switch ( tp )
    {
    case SHT_NULL:
        tp_str = "NULL (unused)";
        break;
    case SHT_PROGBITS:
        tp_str = "Program data";
        break;
    case SHT_SYMTAB:
        tp_str = "Symbol table";
        break;
    case SHT_STRTAB:
        tp_str = "String table";
        break;
    case SHT_RELA:
        tp_str = "Relocation entries with addends";
        break;
    case SHT_HASH:
        tp_str = "Symbol hash table";
        break;
    case SHT_DYNAMIC:
        tp_str = "Dynamic linking information";
        break;
    case SHT_NOTE:
        tp_str = "Notes";
        break;
    case SHT_NOBITS:
        tp_str = "Program space with no data (bss)";
        break;
    case SHT_REL:
        tp_str = "Relocation entries, no addends";
        break;
    case SHT_DYNSYM:
        tp_str = "Dynamic linker symbol table";
        break;
    case SHT_INIT_ARRAY:
        tp_str = "Array of constructors";
        break;
    case SHT_GROUP:
        tp_str = "Section group";
        break;
    }
    return tp_str;
}

