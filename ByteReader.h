//
// Created by Weilai Xiang on 21/03/29.
//

#ifndef IMAGE_BYTEREADER_H
#define IMAGE_BYTEREADER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class ByteReader {
private:
    std::string filepath;
    std::vector<uint8_t> bytes;
public:
    explicit ByteReader(const std::string &path);

    uint8_t getByte(unsigned int pos);

    std::string getBytes(unsigned int begin, unsigned int end);

    int getIntLittleEndian(unsigned int begin);

    unsigned int getUIntLittleEndian(unsigned int begin);

    unsigned int getUIntBigEndian(unsigned int begin);

    uint16_t getUShortBigEndian(unsigned int begin);
};

#endif //IMAGE_BYTEREADER_H
