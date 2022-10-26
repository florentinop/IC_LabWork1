//
// Created by diogo on 11/10/22.
//

#include "BitStream.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if(argc < 3) {
        cerr << "Usage: " << argv[0] << " <text file> <binary file>\n";
        return 1;
    }

    ifstream readStream;
    readStream.open(argv[1]);
    BitStream writeStream {argv[2]};

    char res;
    while (readStream >> res) {
        writeStream.writeBit(res);
    }

    readStream.close();

    return 0;
}
