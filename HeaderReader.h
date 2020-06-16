#ifndef HEADERREADER_H
#define HEADERREADER_H

#include <vector>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <istream>

class HeaderParser final
{
public:
    enum EndianNess { EndianBig, EndianLittle };
    HeaderParser(std::vector<uint8_t> const &hdr_data, EndianNess endian_ness);
    ~HeaderParser();
    void reset();
    size_t getSize();
    uint8_t parseu8();
    uint16_t parseu16();
    uint32_t parseu32();
    uint64_t parseu64();
private:
    std::vector<uint8_t> const &hdr_data_;
    size_t offset_ = 0;
    EndianNess endianness_;
};

template < class H, size_t sz >
class HeaderExtractor
{
public:
    typedef H Hdr; // Type of header that is being extracted
    enum { read_size = sz };

    HeaderExtractor(std::vector<uint8_t> const &hdr_data, HeaderParser::EndianNess en,
                    std::function<void(HeaderParser &, H &, size_t)> fn)
    : hdr_data_(hdr_data),
      endianness_(en),
      fn_(fn)
    {
    }

    ~HeaderExtractor()
    {
    }

    void extractHdr(H &header)
    {
        HeaderParser hp(hdr_data_, endianness_);
        fn_(hp, header, sz);
    }

private:
    std::vector<uint8_t> const &hdr_data_;
    HeaderParser::EndianNess endianness_;
    std::function<void(HeaderParser &hp, H &hdr, size_t size)> fn_;
};

class HeaderReader
{
public:
    HeaderReader(HeaderParser::EndianNess eness);
    virtual ~HeaderReader();
    void readHdr(std::istream &is);
protected:
    HeaderParser::EndianNess eness_;
    std::vector<uint8_t> buf_;
private:
    void readBuf(std::istream &is, ssize_t n);
    virtual ssize_t getSize() = 0;
    virtual void extractHdr() = 0;
};


#endif // HEADERREADER_H
