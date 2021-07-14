//
// Created by Weilai Xiang on 21/03/29.
//

#include "ByteReader.h"

ByteReader::ByteReader(const std::string &path) {
    filepath = path;
    std::ifstream file(filepath.c_str(), std::ios::binary);
    char c;
    while (file.get(c)) {
        bytes.push_back(c);
    }
}

uint8_t ByteReader::getByte(unsigned int pos) {
    return bytes[pos];
}

std::string ByteReader::getBytes(unsigned int begin, unsigned int end) {
    std::string res;
    for (unsigned int i = begin; i < end; ++i) {
        res += bytes[i];
    }
    return res;
}

int ByteReader::getIntLittleEndian(unsigned int begin) {
    int res = getByte(begin) & 0xFF;
    res |= (getByte(begin + 1) << 8) & 0xFF00;
    res |= (getByte(begin + 2) << 16) & 0xFF0000;
    res |= (getByte(begin + 3) << 24) & 0xFF000000;
    return res;
}

unsigned int ByteReader::getUIntLittleEndian(unsigned int begin) {
    unsigned int res = getByte(begin) & 0xFF;
    res |= (getByte(begin + 1) << 8) & 0xFF00;
    res |= (getByte(begin + 2) << 16) & 0xFF0000;
    res |= (getByte(begin + 3) << 24) & 0xFF000000;
    return res;
}

unsigned int ByteReader::getUIntBigEndian(unsigned int begin) {
    unsigned int res = getByte(begin);
    res <<= 8;
    res |= getByte(begin + 1);
    res <<= 8;
    res |= getByte(begin + 2);
    res <<= 8;
    res |= getByte(begin + 3);
    return res;
}

uint16_t ByteReader::getUShortBigEndian(unsigned int begin) {
    uint16_t res = getByte(begin);
    res <<= 8;
    res |= getByte(begin + 1);
    return res;
}
