//
// Created by diogo on 11/10/22.
//

#ifndef SNDFILE_EXAMPLE_BITSTREAM_H
#define SNDFILE_EXAMPLE_BITSTREAM_H


class BitStream {
private:
    char* path;
    unsigned int bitPointer;

public:
    BitStream(char* path, unsigned int bitPointer) : BitStream(path) {
        this->path = path;
        this->bitPointer = bitPointer;
    }

    BitStream(char* path) : BitStream(path, 0){}

    unsigned char readBit() {
        char res;
        ifstream infile;
        infile.open(path);
        infile.seekg(bitPointer>>3); // basically the same as bitPointer/8, just faster maybe?
        if (infile.get(res)) {
            res >>= (7 - (bitPointer % 8));
            res &= 0b00000001;
        }
        infile.close();
        bitPointer++;
        return res;
    }

    void setBitPointer(unsigned int bit) {
        bitPointer = bit;
    }
};


#endif //SNDFILE_EXAMPLE_BITSTREAM_H
