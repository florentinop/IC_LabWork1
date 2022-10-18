//
// Created by diogo on 11/10/22.
//

#include "BitStream.h"
#include <iostream>

int main() {
    BitStream readStream {"in.txt"};
    BitStream writeStream {"out.txt"};

    vector<unsigned char> bitsRead = readStream.readNBits(8);
    for (auto c: bitsRead) {
        cout << c;
    }
    unsigned char bit = readStream.readBit();
    cout << bit;

    writeStream.writeNBits(bitsRead);
    writeStream.writeBit(bit);

    writeStream.writeNBits(readStream.readNBits(8));

    return 0;
}
