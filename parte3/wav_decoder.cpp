//
// Created by diogo on 26/10/22.
//

#include <iostream>
#include <vector>
#include <fftw3.h>
#include <sndfile.hh>
#include <bitset>
#include "../parte2/BitStream.h"

using namespace std;

int main(int argc, char *argv[]) {

    bool verbose{false};

    if (argc < 3) {
        cerr << "Usage: wav_decoder binFileIn wavFileOut\n";
        return 1;
    }

    BitStream readStream { argv[argc-2] };



    SndfileHandle sfhOut { argv[argc-1], SFM_WRITE, 2^bits,
                           sfhIn.channels(), sfhIn.samplerate() };

    sfhOut.writef(samples.data(), frames());

    return 0;
}