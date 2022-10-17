//
// Created by diogo on 11/10/22.
//

#include "BitStream.h"
#include <iostream>

int main() {
    string path = "test.txt";
    BitStream bitStream {path};
    // std::cout << (bitStream.readBit() == '\0' ? 0 : 1) << (bitStream.readBit() == '\0' ? 0 : 1) << (bitStream.readBit() == '\0' ? 0 : 1) << (bitStream.readBit() == '\0' ? 0 : 1) << (bitStream.readBit() == '\0' ? 0 : 1) << (bitStream.readBit() == '\0' ? 0 : 1) << (bitStream.readBit() == '\0' ? 0 : 1) << (bitStream.readBit() == '\0' ? 0 : 1) << (bitStream.readBit() == '\0' ? 0 : 1) << std::endl;
//    bitStream.writeBit(0);
//    bitStream.writeBit(1);
//    bitStream.writeBit(0);
//    bitStream.writeBit(0);
//    bitStream.writeBit(0);
//    bitStream.writeBit(1);
//    bitStream.writeBit(1);
//    bitStream.writeBit(1);
    vector<unsigned char> vec = bitStream.readNBits(8);
    std::cout << (vec[0] == 0 ? '0' : '1') << (vec[1] == 0 ? '0' : '1') << (vec[2] == 0 ? '0' : '1') << (vec[3] == 0 ? '0' : '1') << (vec[4] == 0 ? '0' : '1') << (vec[5] == 0 ? '0' : '1') << (vec[6] == 0 ? '0' : '1') << (vec[7] == 0 ? '0' : '1') << std::endl;
    return 0;
}
