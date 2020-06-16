#include <iostream>
#include <istream>
#include <sstream>
#include "HeaderReader.h"
#include "HeaderWriter.h"

void test00();
void test00EHelper(std::string const &hdr, std::vector<uint8_t> const &data, HeaderParser::EndianNess eness);
void test00SHelper(std::string const &hdr, std::vector<uint8_t> const &data, HeaderParser::EndianNess eness);
struct Hdr00
{
    uint16_t a;
    uint16_t b;
    uint8_t c;
};
constexpr size_t Hdr00RealSize = 5;
void Hdr00Extract(HeaderParser &hp, Hdr00 &hdr, size_t size);
class Hdr00Reader : public HeaderReader
{
public:
    Hdr00Reader(HeaderParser::EndianNess eness);
    virtual ~Hdr00Reader();
    void getHdr(Hdr00 &h);
private:
    virtual ssize_t getSize();
    virtual void extractHdr();
    Hdr00 h_;
};

void test01();
void test01SHelper(std::string const &heading, std::vector<uint8_t> const &data, HeaderParser::EndianNess eness);
struct Hdr01
{
    uint32_t a;
    uint64_t b;
    uint16_t c;
};
void Hdr01Extract(HeaderParser &hp, Hdr01 &hdr, size_t size);
constexpr size_t Hdr01RealSize = 14;
class Hdr01Reader : public HeaderReader
{
public:
    Hdr01Reader(HeaderParser::EndianNess eness);
    virtual ~Hdr01Reader();
    void getHdr(Hdr01 &h);
private:
    virtual ssize_t getSize();
    virtual void extractHdr();
    Hdr01 h_;
};

int main()
{
    std::cout << "Hey!\n";
    test00();
    test01();
    return 0;
}


void test00()
{
    std::cout << "test00()\n";
    std::vector<uint8_t> data_a { 0xfa, 0x31, 0x46, 0xc7, 0x94 };
    test00SHelper("data_a Big", data_a, HeaderParser::EndianBig);
    test00SHelper("data_a Lit", data_a, HeaderParser::EndianLittle);
}

void test01()
{
    std::cout << "test01()\n";
    std::vector<uint8_t> data_a { 0xeb, 0xc3, 0x42, 0x10,
                                  0x72, 0x27, 0x58, 0x8a, 0x99, 0x01, 0x32, 0x79,
                                  0x11, 0x22 };
    test01SHelper("data_a Big", data_a, HeaderParser::EndianBig);
    test01SHelper("data_a Lit", data_a, HeaderParser::EndianLittle);
}

void test00SHelper(std::string const &heading, std::vector<uint8_t> const &data, HeaderParser::EndianNess eness)
{
    Hdr00 h;
    char *d = (char *)(data.data());
    std::string str(d, d+data.size());
    std::istringstream stream(str);
    Hdr00Reader hdr_rdr(eness);
    hdr_rdr.readHdr(stream);
    hdr_rdr.getHdr(h);
    std::cout << "  (S) " << heading << ":  h.a==" << h.a << ", h.b==" << h.b << ", h.c==" << unsigned(h.c) << "\n";
}

void test01SHelper(std::string const &heading, std::vector<uint8_t> const &data, HeaderParser::EndianNess eness)
{
    Hdr01 h;
    char *d = (char *)(data.data());
    std::string str(d, d+data.size());
    std::istringstream stream(str);
    Hdr01Reader hdr_rdr(eness);
    hdr_rdr.readHdr(stream);
    hdr_rdr.getHdr(h);
    std::cout << "  (S) " << heading << ":  h.a==" << h.a << ", h.b==" << h.b << ", h.c==" << unsigned(h.c) << "\n";
}

void Hdr00Extract(HeaderParser &hp, Hdr00 &hdr, size_t size)
{
    if ( size != sizeof(uint16_t)+sizeof(uint16_t)+sizeof(uint8_t) )
        throw std::out_of_range("Hdr00Extract() given mismatching size");
    if ( hp.getSize() < size )
        throw std::out_of_range("Hdr00Extract() data is too small");
    hdr.a = hp.parseu16();
    hdr.b = hp.parseu16();
    hdr.c = hp.parseu8();
}

void Hdr01Extract(HeaderParser &hp, Hdr01 &hdr, size_t size)
{
    if ( size != sizeof(uint32_t)+sizeof(uint64_t)+sizeof(uint16_t) )
        throw std::out_of_range("Hdr01Extract() given mismatching size");
    if ( hp.getSize() < size )
        throw std::out_of_range("Hdr01Extract() data is too small");
    hdr.a = hp.parseu32();
    hdr.b = hp.parseu64();
    hdr.c = hp.parseu16();
}

Hdr00Reader::Hdr00Reader(HeaderParser::EndianNess eness)
: HeaderReader(eness)
{
}

Hdr00Reader::~Hdr00Reader()
{
}

ssize_t Hdr00Reader::getSize()
{
    return Hdr00RealSize;
}

void Hdr00Reader::extractHdr()
{
    HeaderExtractor<Hdr00, Hdr00RealSize> he(buf_, eness_, Hdr00Extract);
    he.extractHdr(h_);
}

void Hdr00Reader::getHdr(Hdr00 &h)
{
    h = h_;
}

Hdr01Reader::Hdr01Reader(HeaderParser::EndianNess eness)
: HeaderReader(eness)
{
}

Hdr01Reader::~Hdr01Reader()
{
}

ssize_t Hdr01Reader::getSize()
{
    return Hdr01RealSize;
}

void Hdr01Reader::extractHdr()
{
    HeaderExtractor<Hdr01, Hdr01RealSize> he(buf_, eness_, Hdr01Extract);
    he.extractHdr(h_);
}

void Hdr01Reader::getHdr(Hdr01 &h)
{
    extractHdr();
    h = h_;
}

