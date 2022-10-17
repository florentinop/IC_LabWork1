//
// Created by diogo on 11/10/22.
//

#ifndef SNDFILE_EXAMPLE_BITSTREAM_H
#define SNDFILE_EXAMPLE_BITSTREAM_H

#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class BitStream {
private:
    string path;
    unsigned int bitPointer;
    vector<unsigned char> buffer;

public:
    BitStream(string path, unsigned int bitPointer) {
        this->path = path;
        this->bitPointer = bitPointer;
    }

    BitStream(string path) : BitStream(path, 0) {}

    unsigned char readBit() {
        char res;
        ifstream infile;
        infile.open(path);
        infile.seekg(bitPointer>>3); // basically the same as bitPointer/8, just faster maybe?
        if (infile >> res) {
            res >>= 7 - (bitPointer % 8);
            res &= 0b00000001;
        }
        infile.close();
        bitPointer++;
        return res;
    }

    void writeBit(unsigned char bit) {
        bit &= 0b00000001;
        buffer.push_back(bit);
        if (buffer.size() >= 8) {
            char res = 0;
            for (unsigned int i = 0; i < 8; i++) {
                res += (buffer[i] << (7-i));
            }
            ofstream outfile;
            outfile.open(path, ios::out | ios::app);
            outfile << res;
            outfile.close();
        }
    }

    void setBitPointer(unsigned int bit) {
        bitPointer = bit;
    }
};


#endif //SNDFILE_EXAMPLE_BITSTREAM_H
