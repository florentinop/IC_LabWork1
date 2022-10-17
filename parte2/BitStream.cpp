//
// Created by diogo on 11/10/22.
//

#include "BitStream.h"
#include <iostream>

int main() {
    string path = "test.txt";
    BitStream bitStream {path};
    // std::cout << (bitStream.readBit() == '\0' ? 0 : 1) << (bitStream.readBit() == '\0' ? 0 : 1) << (bitStream.readBit() == '\0' ? 0 : 1) << (bitStream.readBit() == '\0' ? 0 : 1) << (bitStream.readBit() == '\0' ? 0 : 1) << (bitStream.readBit() == '\0' ? 0 : 1) << (bitStream.readBit() == '\0' ? 0 : 1) << (bitStream.readBit() == '\0' ? 0 : 1) << (bitStream.readBit() == '\0' ? 0 : 1) << std::endl;
    bitStream.writeBit(0);
    bitStream.writeBit(1);
    bitStream.writeBit(0);
    bitStream.writeBit(0);
    bitStream.writeBit(0);
    bitStream.writeBit(1);
    bitStream.writeBit(1);
    bitStream.writeBit(1);
    return 0;
}
