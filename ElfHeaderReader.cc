#include "ElfHeaderReader.h"

void ElfHeaderExtract(HeaderParser &hp, Elf64Header &hdr, size_t size)
{
    if ( size != ElfHeaderRealSize )
        throw std::out_of_range("ElfHeaderExtract() given mismatching size");
    if ( hp.getSize() < size )
        throw std::out_of_range("ElfHeaderExtract() data is too small");
    for ( size_t i = 0;  i < EI_NIDENT;  ++i )
        hdr.e_ident[i] = hp.parseu8();
    hdr.e_type = hp.parseu16();
    hdr.e_machine = hp.parseu16();
    hdr.e_version = hp.parseu32();
    hdr.e_entry = hp.parseu64();
    hdr.e_phoff = hp.parseu64();
    hdr.e_shoff = hp.parseu64();
    hdr.e_flags = hp.parseu32();
    hdr.e_ehsize = hp.parseu16();
    hdr.e_phentsize = hp.parseu16();
    hdr.e_phnum = hp.parseu16();
    hdr.e_shentsize = hp.parseu16();
    hdr.e_shnum = hp.parseu16();
    hdr.e_shstrndx = hp.parseu16();
}

ElfHeaderReader::ElfHeaderReader(HeaderParser::EndianNess eness)
: HeaderReader(eness)
{
}

ElfHeaderReader::~ElfHeaderReader()
{
}

ssize_t ElfHeaderReader::getSize()
{
    return ElfHeaderRealSize;
}

void ElfHeaderReader::extractHdr()
{
    HeaderExtractor<Elf64Header, ElfHeaderRealSize> he(buf_, eness_, ElfHeaderExtract);
    he.extractHdr(h_);
}

void ElfHeaderReader::getHdr(Elf64Header &h)
{
    h = h_;
}

void Elf64ShdrExtract(HeaderParser &hp, Elf64Shdr &hdr, size_t size)
{
    if ( size != Elf64ShdrRealSize )
        throw std::out_of_range("Elf64ShdrExtract() given mismatching size");
    if ( hp.getSize() < size )
        throw std::out_of_range("Elf64ShdrExtract() data is too small");
    hdr.sh_name = hp.parseu32();
    hdr.sh_type = hp.parseu32();
    hdr.sh_flags = hp.parseu64();
    hdr.sh_addr = hp.parseu64();
    hdr.sh_offset = hp.parseu64();
    hdr.sh_size = hp.parseu64();
    hdr.sh_link = hp.parseu32();
    hdr.sh_info = hp.parseu32();
    hdr.sh_addralign = hp.parseu64();
    hdr.sh_entsize = hp.parseu64();
}

#if 0
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
#endif

Elf64ShdrReader::Elf64ShdrReader(HeaderParser::EndianNess eness)
: HeaderReader(eness)
{
}

Elf64ShdrReader::~Elf64ShdrReader()
{
}

void Elf64ShdrReader::getHdr(Elf64Shdr &h)
{
    h = h_;
}

ssize_t Elf64ShdrReader::getSize()
{
    return Elf64ShdrRealSize;
}

void Elf64ShdrReader::extractHdr()
{
    HeaderExtractor<Elf64Shdr, Elf64ShdrRealSize> he(buf_, eness_, Elf64ShdrExtract);
    he.extractHdr(h_);
}

ElfSection::ElfSection()
{
}

ElfSection::~ElfSection()
{
}

void ElfSection::reset()
{
    data_.clear();
}

void ElfSection::reserve(size_t alloc_sz)
{
    data_.reserve(alloc_sz);
}

void ElfSection::append(std::istream &is, uint64_t pos, uint64_t size)
{
    std::vector<uint8_t> buf(size);
    is.seekg(pos);
    char *c_buf = (char *)buf.data();
    is.read(c_buf, size);
    data_.insert(data_.end(), buf.begin(), buf.end());
}

uint8_t const *ElfSection::data() const
{
    return data_.data();
}

size_t ElfSection::size() const
{
    return data_.size();
}

#if 0
class ElfStringTable final
{
public:
    ElfStringTable();
    ~ElfStringTable();
    void buildFromElfSection(elfSection const &es);
    size_t getNumStrings() const;
    std::string getString(size_t n) const;
private:
    std::vector<std::string> str_vec_;
};
#endif

ElfStringTable::ElfStringTable()
{
}

ElfStringTable::~ElfStringTable()
{
}

void ElfStringTable::buildFromElfSection(ElfSection const &es)
{
    uint8_t const *data = es.data();
    size_t sz = es.size();
    if ( sz == 0 )
        throw std::out_of_range("ElfStringTable::buildFromElfSection() got unexpected empty data in elf section");
    if ( data[0] != 0 )
        throw std::out_of_range("ElfStringTable::buildFromElfSection() missing expected zero at start");
    std::string build_str;
    for ( size_t i = 0;  i < sz;  ++i )
    {
        if ( data[i] == 0 )
        {
            str_vec_.push_back(build_str);
            build_str.clear();
        }
        else
        {
            build_str += char(data[i]);
        }
    }
    if ( !build_str.empty() )
        str_vec_.push_back(build_str);
}

size_t ElfStringTable::getNumStrings() const
{
    return str_vec_.size();
}

std::string ElfStringTable::getString(size_t n) const
{
    if ( n >= str_vec_.size() )
        throw std::out_of_range("ElfStringTable::getString() out of bounds index");
    return str_vec_[n];
}

