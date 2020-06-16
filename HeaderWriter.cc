#include "HeaderWriter.h"

#include <array>
#include <iostream>

HeaderPlacer::HeaderPlacer(std::vector<uint8_t> &hdr_data, EndianNess endian_ness)
: hdr_data_(hdr_data),
  endianness_(endian_ness)
{
}

HeaderPlacer::~HeaderPlacer()
{
}

void HeaderPlacer::reset()
{
    offset_ = 0;
}

size_t HeaderPlacer::getSize()
{
    return hdr_data_.size();
}

void HeaderPlacer::placeu8(uint8_t val)
{
    hdr_data_.push_back(val);
}

void HeaderPlacer::placeu16(uint16_t val)
{
    uint8_t *val_data = reinterpret_cast<uint8_t *>(&val);
    if ( endianness_== EndianBig )
    {
        hdr_data_.push_back(val_data[1]);
        hdr_data_.push_back(val_data[0]);
    }
    else
    {
        hdr_data_.push_back(val_data[0]);
        hdr_data_.push_back(val_data[1]);
    }
}

void HeaderPlacer::placeu32(uint32_t val)
{
    uint8_t *val_data = reinterpret_cast<uint8_t *>(&val);
    if ( endianness_== EndianBig )
    {
        hdr_data_.push_back(val_data[3]);
        hdr_data_.push_back(val_data[2]);
        hdr_data_.push_back(val_data[1]);
        hdr_data_.push_back(val_data[0]);
    }
    else
    {
        hdr_data_.push_back(val_data[0]);
        hdr_data_.push_back(val_data[1]);
        hdr_data_.push_back(val_data[2]);
        hdr_data_.push_back(val_data[3]);
    }
}

void HeaderPlacer::placeu64(uint64_t val)
{
    uint8_t *val_data = reinterpret_cast<uint8_t *>(&val);
    if ( endianness_== EndianBig )
    {
        hdr_data_.push_back(val_data[7]);
        hdr_data_.push_back(val_data[6]);
        hdr_data_.push_back(val_data[5]);
        hdr_data_.push_back(val_data[4]);
        hdr_data_.push_back(val_data[3]);
        hdr_data_.push_back(val_data[2]);
        hdr_data_.push_back(val_data[1]);
        hdr_data_.push_back(val_data[0]);
    }
    else
    {
        hdr_data_.push_back(val_data[0]);
        hdr_data_.push_back(val_data[1]);
        hdr_data_.push_back(val_data[2]);
        hdr_data_.push_back(val_data[3]);
        hdr_data_.push_back(val_data[4]);
        hdr_data_.push_back(val_data[5]);
        hdr_data_.push_back(val_data[6]);
        hdr_data_.push_back(val_data[7]);
    }
}

HeaderWriter::HeaderWriter(HeaderPlacer::EndianNess eness)
: eness_(eness)
{
}

HeaderWriter::~HeaderWriter()
{
}

void HeaderWriter::writeHdr(std::istream &is)
{
    writeBuf(is, getSize());
    emitHdr();
}

void HeaderWriter::writeBuf(std::istream &is, ssize_t n)
{
    buf_.resize(n);
    char *data = (char *)(buf_.data());
    is.read(data, n);
    auto gc = is.gcount();
    if ( gc != n )
        throw std::logic_error("Failed to read correct number of bytes in HeaderWriter::writeBuf()");
}
