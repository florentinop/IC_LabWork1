//
// Created by diogo on 11/10/22.
//

#include "BitStream.h"
#include <iostream>

void readBits(BitStream rd, int n) {
    vector<unsigned char> bitsRead = rd.readNBits(n);
    for (auto c : bitsRead)
        cout << (int)c;
}

int main() {
    BitStream readStream {"in.txt"};
    BitStream writeStream {"out.txt"};
    // readBits(readStream,16);

    vector<unsigned char> chars = {'o','l','a'};
    writeStream.writeNBits(chars);
    readBits(writeStream,24);

    
    // cout << bit;

    // writeStream.writeNBits(bitsRead);
    // writeStream.writeBit(bit);

    // writeStream.writeNBits(readStream.readNBits(8));

    return 0;
}
