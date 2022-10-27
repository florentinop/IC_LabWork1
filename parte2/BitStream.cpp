//
// Created by diogo on 11/10/22.
//

#include "BitStream.h"
#include <iostream>

// void readBits(BitStream rd, int n) {
//     vector<unsigned char> bitsRead = rd.readBits(n);
//     for (auto c : bitsRead)
//         cout << (int)c;
// }

void encode(string text, string bin){
    BitStream readStream {text};
    BitStream writeStream {bin};

    char c = ' ';
    while(1){
        c = readStream.readBit();
        if(c == EOF)
            break;
        
        writeStream.writeBit(c);
    }
}

void decode(string bin, string text){
    BitStream readStream {bin};
    BitStream writeStream {text};

    char c = ' ';
    while(1){
        c = readStream.readBit();
        if(c == EOF)
            break;
        
        writeStream.writeChar(c);
    }
}



int main(int argc, char *argv[]) {

    if(argc < 3) {
		cerr << "Usage: " << argv[0] << " <text file> <binary file>\n";
		return 1;
	}

    encode(argv[1],argv[2]);
    
    

    return 0;
}
