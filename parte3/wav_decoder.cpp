//
// Created by diogo on 26/10/22.
//

#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <bitset>
#include "../parte2/BitStream.h"

using namespace std;

int main(int argc, char *argv[]) {

    if (argc < 3) {
        cerr << "Usage: wav_decoder binFileIn wavFileOut\n";
        return 1;
    }

    BitStream readStream { argv[argc-2] };

    const clock_t begin = clock();

    int channels = (unsigned char) readStream.readBit();
    if (channels == -1) {
        cerr << "Error: invalid input file\n";
        return 1;
    }
    channels = (channels == 1 ? 2 : 1);

    string depthBits = readStream.readBits(4);
    if (depthBits.size() < 4) {
        cerr << "Error: invalid input file\n";
        return 1;
    }
    int depth = stoi(depthBits, nullptr, 2) + 1;

    string samplerateBits = readStream.readBits(19);
    if (samplerateBits.size() < 19) {
        cerr << "Error: invalid input file\n";
        return 1;
    }
    int samplerate = stoi(samplerateBits, nullptr, 2);

    vector<short> samples;
    string s;
    while ((s = readStream.readBits(depth)).size() >= depth) {
        samples.push_back((short) stoi(s, nullptr, 2));
    }

    SndfileHandle sfhOut { argv[argc-1], SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, channels, samplerate };
    if(sfhOut.error()) {
        cerr << "Error generating output file\n";
        return 1;
    }
    sfhOut.writef(samples.data(), (int) samples.size()/2);

    cout << "Time in seconds: ";
    cout << float(clock() - begin) / CLOCKS_PER_SEC << '\n';

    return 0;
}