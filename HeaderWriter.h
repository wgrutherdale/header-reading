#ifndef HEADERWRITER_H
#define HEADERWRITER_H

#include <vector>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <istream>

class HeaderPlacer final
{
public:
    enum EndianNess { EndianBig, EndianLittle };
    HeaderPlacer(std::vector<uint8_t> &hdr_data, EndianNess endian_ness);
    ~HeaderPlacer();
    void reset();
    size_t getSize();
    void placeu8(uint8_t val);
    void placeu16(uint16_t val);
    void placeu32(uint32_t val);
    void placeu64(uint64_t val);
private:
    std::vector<uint8_t> &hdr_data_;
    size_t offset_ = 0;
    EndianNess endianness_;
};

template < class H, size_t sz >
class HeaderEmitter
{
public:
    typedef H Hdr; // Type of header that is being emitted
    enum { read_size = sz };

    HeaderEmitter(std::vector<uint8_t> &hdr_data, HeaderPlacer::EndianNess en,
                    std::function<void(HeaderPlacer &, H const&, size_t)> fn)
    : hdr_data_(hdr_data),
      endianness_(en),
      fn_(fn)
    {
    }

    ~HeaderEmitter()
    {
    }

    void emitHdr(H &header)
    {
        HeaderPlacer hp(hdr_data_, endianness_);
        fn_(hp, header, sz);
    }

private:
    std::vector<uint8_t> &hdr_data_;
    HeaderPlacer::EndianNess endianness_;
    std::function<void(HeaderPlacer &hp, H &hdr, size_t size)> fn_;
};

class HeaderWriter
{
public:
    HeaderWriter(HeaderPlacer::EndianNess eness);
    virtual ~HeaderWriter();
    void writeHdr(std::istream &is);
protected:
    HeaderPlacer::EndianNess eness_;
    std::vector<uint8_t> buf_;
private:
    void writeBuf(std::istream &is, ssize_t n);
    virtual ssize_t getSize() = 0;
    virtual void emitHdr() = 0;
};


#endif // HEADERWRITER_H
