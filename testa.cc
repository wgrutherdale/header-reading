#include <iostream>
#include "HeaderReader.h"
#include "HeaderWriter.h"

void test00();
void test00W();
void test00Helper(std::string const &hdr, std::vector<uint8_t> const &data, HeaderParser::EndianNess eness);
void test00EHelper(std::string const &hdr, std::vector<uint8_t> const &data, HeaderParser::EndianNess eness);
void test00WHelper(std::string const &hdr, HeaderPlacer::EndianNess eness);
struct Hdr00
{
    uint16_t a;
    uint16_t b;
    uint8_t c;
};
constexpr size_t Hdr00RealSize = 5;
void Hdr00Extract(HeaderParser &hp, Hdr00 &hdr, size_t size);

void test01();
void test01W();
void test01Helper(std::string const &hdr, std::vector<uint8_t> const &data, HeaderParser::EndianNess eness);
void test01EHelper(std::string const &hdr, std::vector<uint8_t> const &data, HeaderParser::EndianNess eness);
void test01WHelper(std::string const &hdr, HeaderPlacer::EndianNess eness);
struct Hdr01
{
    uint32_t a;
    uint64_t b;
    uint16_t c;
};
void Hdr01Extract(HeaderParser &hp, Hdr01 &hdr, size_t size);
constexpr size_t Hdr01RealSize = 14;

int main()
{
    std::cout << "Hey!\n";
    test00();
    test00W();
    test01();
    test01W();
    return 0;
}


void test00()
{
    std::cout << "test00()\n";
    std::vector<uint8_t> data_a { 0xfa, 0x31, 0x46, 0xc7, 0x94 };
    test00Helper("data_a Big", data_a, HeaderParser::EndianBig);
    test00Helper("data_a Lit", data_a, HeaderParser::EndianLittle);
    test00EHelper("data_a Big", data_a, HeaderParser::EndianBig);
    test00EHelper("data_a Lit", data_a, HeaderParser::EndianLittle);
}

void test00W()
{
    std::cout << "test00W()\n";
    test00WHelper("data Big", HeaderPlacer::EndianBig);
    test00WHelper("data Lit", HeaderPlacer::EndianLittle);
}

void test01()
{
    std::cout << "test01()\n";
    std::vector<uint8_t> data_a { 0xeb, 0xc3, 0x42, 0x10,
                                  0x72, 0x27, 0x58, 0x8a, 0x99, 0x01, 0x32, 0x79,
                                  0x11, 0x22 };
    test01Helper("data_a Big", data_a, HeaderParser::EndianBig);
    test01Helper("data_a Lit", data_a, HeaderParser::EndianLittle);
    test01EHelper("data_a Big", data_a, HeaderParser::EndianBig);
    test01EHelper("data_a Lit", data_a, HeaderParser::EndianLittle);
}

void test01W()
{
    std::cout << "test01W()\n";
    test01WHelper("data Big", HeaderPlacer::EndianBig);
    test01WHelper("data Lit", HeaderPlacer::EndianLittle);
}

void test00Helper(std::string const &hdr, std::vector<uint8_t> const &data, HeaderParser::EndianNess eness)
{
    HeaderParser hp(data, eness);
    Hdr00 h;
    h.a = hp.parseu16();
    h.b = hp.parseu16();
    h.c = hp.parseu8();
    std::cout << "  " << hdr << ":  h.a==" << h.a << ", h.b==" << h.b << ", h.c==" << unsigned(h.c) << "\n";
}

void test00EHelper(std::string const &heading, std::vector<uint8_t> const &data, HeaderParser::EndianNess eness)
{
    Hdr00 h;
    std::cout << "  Hdr00 size is " << sizeof(Hdr00) << "\n";
    HeaderExtractor<Hdr00, Hdr00RealSize> he(data, eness, Hdr00Extract);
    he.extractHdr(h);
    std::cout << "  (E) " << heading << ":  h.a==" << h.a << ", h.b==" << h.b << ", h.c==" << unsigned(h.c) << "\n";
}

void dumpUint8Vec(std::ostream &os, std::vector<uint8_t> const &uv)
{
    os << "{";
    for ( auto u8 : uv )
        //os << std::cout.hex << unsigned(u8) << ",";
        os << unsigned(u8) << ",";
    os << "}";
}

void test00WHelper(std::string const &heading, HeaderPlacer::EndianNess eness)
{
    std::vector<uint8_t> data;
    HeaderPlacer hp(data, eness);
    hp.placeu16(31546u);
    hp.placeu16(4421);
    hp.placeu8(230);
    std::cout << "  " << heading << ":  data==";
    dumpUint8Vec(std::cout, data);
    std::cout << "\n";
}

void test01Helper(std::string const &hdr, std::vector<uint8_t> const &data, HeaderParser::EndianNess eness)
{
    HeaderParser hp(data, eness);
    Hdr01 h;
    h.a = hp.parseu32();
    h.b = hp.parseu64();
    h.c = hp.parseu16();
    std::cout << "  " << hdr << ":  h.a==" << h.a << ", h.b==" << h.b << ", h.c==" << unsigned(h.c) << "\n";
}
void test01EHelper(std::string const &heading, std::vector<uint8_t> const &data, HeaderParser::EndianNess eness)
{
    Hdr01 h;
    std::cout << "  Hdr01 size is " << sizeof(Hdr01) << "\n";
    HeaderExtractor<Hdr01, Hdr01RealSize> he(data, eness, Hdr01Extract);
    he.extractHdr(h);
    std::cout << "  (E) " << heading << ":  h.a==" << h.a << ", h.b==" << h.b << ", h.c==" << unsigned(h.c) << "\n";
}

void test01WHelper(std::string const &hdr, HeaderPlacer::EndianNess eness)
{
    std::vector<uint8_t> data;
    HeaderPlacer hp(data, eness);
    hp.placeu32(1670350912u); // 99, 143, 136, 64 => 63, 8f, 88, 40
    hp.placeu64(0xd43c0a0fa810df70LU); // 212, 60, 10, 15, 168, 16, 223, 112 => d4, 3c 0a, 0f, a8, 10, df, 70
    hp.placeu16(0xef7fu); // 239, 127 => ef, 7f
    std::cout << "  " << hdr << ":  data==";
    dumpUint8Vec(std::cout, data);
    std::cout << "\n";
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

