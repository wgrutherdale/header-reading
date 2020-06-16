#include "HeaderReader.h"

#include <array>
#include <iostream>

HeaderParser::HeaderParser(std::vector<uint8_t> const &hdr_data, EndianNess endian_ness)
: hdr_data_(hdr_data),
  endianness_(endian_ness)
{
}

HeaderParser::~HeaderParser()
{
}

void HeaderParser::reset()
{
    offset_ = 0;
}

size_t HeaderParser::getSize()
{
    return hdr_data_.size();
}

uint8_t HeaderParser::parseu8()
{
    uint8_t val = 0;
    if ( offset_+sizeof(uint8_t) <= hdr_data_.size() )
    {
        uint8_t const *data = hdr_data_.data();
        val = data[offset_++];
    }
    return val;
}

uint16_t HeaderParser::parseu16()
{
    uint16_t val = 0;
    if ( offset_+sizeof(uint16_t) <= hdr_data_.size() )
    {
        uint8_t const *data = hdr_data_.data();
        uint16_t val_0 = data[offset_++];
        uint16_t val_1 = data[offset_++];
        val = endianness_== EndianBig ? (val_0<<8)+val_1 : (val_1<<8)+val_0;
    }
    return val;
}

uint32_t HeaderParser::parseu32()
{
    uint32_t val = 0;
    if ( offset_+sizeof(uint32_t) <= hdr_data_.size() )
    {
        uint8_t const *data = hdr_data_.data();
        std::array<uint32_t, sizeof(uint32_t)> val_bytes;
        val_bytes[0] = data[offset_++];
        val_bytes[1] = data[offset_++];
        val_bytes[2] = data[offset_++];
        val_bytes[3] = data[offset_++];
        if ( endianness_==EndianBig )
            val = (val_bytes[0]<<24)+(val_bytes[1]<<16)+(val_bytes[2]<<8)+val_bytes[3];
        else
            val = (val_bytes[3]<<24)+(val_bytes[2]<<16)+(val_bytes[1]<<8)+val_bytes[0];
    }
    return val;
}

uint64_t HeaderParser::parseu64()
{
    uint64_t val = 0;
    if ( offset_+sizeof(uint64_t) <= hdr_data_.size() )
    {
        uint8_t const *data = hdr_data_.data();
        std::array<uint64_t, sizeof(uint64_t)> val_bytes;
        val_bytes[0] = data[offset_++];
        val_bytes[1] = data[offset_++];
        val_bytes[2] = data[offset_++];
        val_bytes[3] = data[offset_++];
        val_bytes[4] = data[offset_++];
        val_bytes[5] = data[offset_++];
        val_bytes[6] = data[offset_++];
        val_bytes[7] = data[offset_++];
        if ( endianness_==EndianBig )
            val = (val_bytes[0]<<56)+(val_bytes[1]<<48)+(val_bytes[2]<<40)+(val_bytes[3]<<32)+
                  (val_bytes[4]<<24)+(val_bytes[5]<<16)+(val_bytes[6]<<8)+val_bytes[7];
        else
            val = (val_bytes[7]<<56)+(val_bytes[6]<<48)+(val_bytes[5]<<40)+(val_bytes[4]<<32)+
                  (val_bytes[3]<<24)+(val_bytes[2]<<16)+(val_bytes[1]<<8)+val_bytes[0];
    }
    return val;
}

HeaderReader::HeaderReader(HeaderParser::EndianNess eness)
: eness_(eness)
{
}

HeaderReader::~HeaderReader()
{
}

void HeaderReader::readHdr(std::istream &is)
{
    readBuf(is, getSize());
    extractHdr();
}

void HeaderReader::readBuf(std::istream &is, ssize_t n)
{
    buf_.resize(n);
    char *data = (char *)(buf_.data());
    is.read(data, n);
    auto gc = is.gcount();
    if ( gc != n )
        throw std::logic_error("Failed to read correct number of bytes in HeaderReader::readBuf()");
}
