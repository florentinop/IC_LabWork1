#include <iostream>
#include <vector>
#include <cmath>
#include <fftw3.h>
#include <sndfile.hh>
#include "../parte2/BitStream.h"

using namespace std;

int main(int argc, char *argv[]) {

	bool verbose { false };
	size_t bs { 1024 };
	double dctFrac { 0.2 };
	short bits { 8 };

	if(argc < 3) {
		cerr << "Usage: wav_encoder [ -v (verbose) ]\n";
		cerr << "                   [ -bs blockSize (def 1024) ]\n";
		cerr << "                   [ -frac dctFraction (def 0.2) ]\n";
		cerr << "                   [ -b bits (def 16) ]\n";
		cerr << "                   wavFileIn binFileOut\n";
		return 1;
	}

	for(int n = 1 ; n < argc ; n++) {
        if (string(argv[n]) == "-v") {
            verbose = true;
            break;
        }
    }

	for(int n = 1 ; n < argc ; n++) {
        if (string(argv[n]) == "-bs") {
            bs = atoi(argv[n + 1]);
            break;
        }
    }

	for(int n = 1 ; n < argc ; n++) {
        if (string(argv[n]) == "-frac") {
            dctFrac = atof(argv[n + 1]);
            break;
        }
    }

    for(int n = 1 ; n < argc ; n++) {
        if (string(argv[n]) == "-b") {
            bits = atof(argv[n + 1]);
            break;
        }
    }

	SndfileHandle sfhIn { argv[argc-2] };
	if(sfhIn.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    }

	if((sfhIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	if((sfhIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}

	if(verbose) {
		cout << "Input file has:\n";
		cout << '\t' << sfhIn.frames() << " frames\n";
		cout << '\t' << sfhIn.samplerate() << " samples per second\n";
		cout << '\t' << sfhIn.channels() << " channels\n";
		cout << '\t' << sfhIn.format() << '\n';
	}

	size_t nChannels { static_cast<size_t>(sfhIn.channels()) };
	size_t nFrames { static_cast<size_t>(sfhIn.frames()) };

	// Read all samples: c1 c2 ... cn c1 c2 ... cn ...
	// Note: A frame is a group c1 c2 ... cn
	vector<short> samples(nChannels * nFrames);
	sfhIn.readf(samples.data(), nFrames);

	size_t nBlocks { static_cast<size_t>(ceil(static_cast<double>(nFrames) / bs)) };

	// Do zero padding, if necessary
	samples.resize(nBlocks * bs * nChannels);

	// Vector for holding all DCT coefficients, channel by channel
	vector<vector<double>> x_dct(nChannels, vector<double>(nBlocks * bs));

	// Vector for holding DCT computations
	vector<double> x(bs);

	// Direct DCT
	fftw_plan plan_d = fftw_plan_r2r_1d(bs, x.data(), x.data(), FFTW_REDFT10, FFTW_ESTIMATE);
	for(size_t n = 0 ; n < nBlocks ; n++) {
        for (size_t c = 0; c < nChannels; c++) {
            for (size_t k = 0; k < bs; k++)
                x[k] = samples[(n * bs + k) * nChannels + c];

            fftw_execute(plan_d);
            // Keep only "dctFrac" of the "low frequency" coefficients
            for (size_t k = 0; k < bs * dctFrac; k++)
                x_dct[c][n * bs + k] = x[k] / (bs << 1);

        }
    }

	// Inverse DCT
	fftw_plan plan_i = fftw_plan_r2r_1d(bs, x.data(), x.data(), FFTW_REDFT01, FFTW_ESTIMATE);
	for(size_t n = 0 ; n < nBlocks ; n++) {
        for (size_t c = 0; c < nChannels; c++) {
            for (size_t k = 0; k < bs; k++)
                x[k] = x_dct[c][n * bs + k];

            fftw_execute(plan_i);
            for (size_t k = 0; k < bs; k++)
                samples[(n * bs + k) * nChannels + c] = static_cast<short>(round(x[k]));

        }
    }

    std::vector<short> res(samples.size());
    int delta = (int) pow(2, 16 - bits);
    for (unsigned long i = 0; i < samples.size(); i++) {
        if (samples[i] % delta >= delta / 2) {
            res[i] = delta*(1+samples[i]/delta);
        } else {
            res[i] = samples[i] - (samples[i] % delta);
        }
    }

    string path {argv[argc-1]};
    BitStream writeStream {path};

    const clock_t begin = clock();

//    for (auto re: res) {
//        re >>= 8;
//        for (unsigned int i = 0; i < 8; i++) {
//            writeStream.writeBit((re >> (7 - i)) & 1);
//        }
//    }
    cout << "res length: " << res.size() << '\n';
    for (auto re: res) {
        re >>= 8;
        vector<char> vec {static_cast<char>((re >> 7) & 1), static_cast<char>((re >> 6) & 1),
                          static_cast<char>((re >> 5) & 1), static_cast<char>((re >> 4) & 1),
                          static_cast<char>((re >> 3) & 1), static_cast<char>((re >> 2) & 1),
                          static_cast<char>((re >> 1) & 1), static_cast<char>(re & 1)};
        writeStream.writeBits(vec);
    }

    cout << "Time in seconds: ";
    cout << float(clock() - begin) / CLOCKS_PER_SEC << '\n';

	return 0;
}

