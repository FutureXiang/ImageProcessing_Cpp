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

std::string ByteReader::getBytes(int begin, int end) {
    std::string res;
    for (int i = begin; i < end; ++i) {
        res += bytes[i];
    }
    return res;
}

int ByteReader::getIntLittleEndian(int begin) {
    int res = getByte(begin) & 0xFF;
    res |= (getByte(begin + 1) << 8) & 0xFF00;
    res |= (getByte(begin + 2) << 16) & 0xFF0000;
    res |= (getByte(begin + 3) << 24) & 0xFF000000;
    return res;
}

unsigned int ByteReader::getUIntLittleEndian(int begin) {
    unsigned int res = getByte(begin) & 0xFF;
    res |= (getByte(begin + 1) << 8) & 0xFF00;
    res |= (getByte(begin + 2) << 16) & 0xFF0000;
    res |= (getByte(begin + 3) << 24) & 0xFF000000;
    return res;
}
