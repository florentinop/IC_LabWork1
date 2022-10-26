//
// Created by diogo on 26/10/22.
//

#include "BitStream.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if(argc < 3) {
        cerr << "Usage: " << argv[0] << " <binary file> <text file>\n";
        return 1;
    }

    BitStream readStream {argv[1]};
    ofstream writeStream;
    writeStream.open(argv[2]);

    unsigned char res;
    while ((res = readStream.readBit()) != '2') {
        writeStream << res;
    }

    writeStream.close();

    return 0;
}

