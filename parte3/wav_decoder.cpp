//
// Created by diogo on 26/10/22.
//

using namespace std;

int main(int argc, char *argv[]) {

    bool verbose{false};
    short bits{8};

    if (argc < 3) {
        cerr << "Usage: wav_decoder [ -v (verbose) ]\n";
        cerr << "                   [ -b bits (def 8) ]\n";
        cerr << "                   binFileIn wavFileOut\n";
        return 1;
    }

    for (int n = 1; n < argc; n++) {
        if (string(argv[n]) == "-v") {
            verbose = true;
            break;
        }
    }

    for (int n = 1; n < argc; n++) {
        if (string(argv[n]) == "-b") {
            bits = atof(argv[n + 1]);
            break;
        }
    }

    SndfileHandle sfhOut { argv[argc-1], SFM_WRITE, 2^bits,
                           sfhIn.channels(), sfhIn.samplerate() };
    if(sfhOut.error()) {
        cerr << "Error: invalid output file\n";
        return 1;
    }

    sfhOut.writef(samples.data(), sfhIn.frames());

    return 0;
}