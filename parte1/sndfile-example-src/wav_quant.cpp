#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_quant.h"


using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {

	if(argc < 4) {
		cerr << "Usage: " << argv[0] << " <input file> <mode={shifting, rounding}> <number of bits>\n";
		return 1;
	}

	SndfileHandle sndFile { argv[argc-3] };

	if(sndFile.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    }

	if((sndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	if((sndFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}

	SndfileHandle outFile { "reduced.wav", SFM_WRITE, sndFile.format(), sndFile.channels(),sndFile.samplerate()};

	size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());

	short nbits=(short)std::stoi(argv[3]);
	if(nbits<1 or nbits >16){
		cerr << "number of bits needs to be between 1 and 16\n";
	}
	
	WAVQuant quant { sndFile };
	while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * sndFile.channels());
		quant.quantize(samples,nbits); //reduce to x bits
		
	}

	string mode = argv[2];

	if(mode=="shifting"){
		vector<short> QuantizedSamples = quant.getQuantizedSamples();
		outFile.writef(QuantizedSamples.data(), sndFile.frames());
	}
	else if(mode=="rounding"){
		outFile.writef(quant.quantize(nbits).data(), sndFile.frames());
	}

	else{
		cerr << "Usage: " << argv[0] << " <input file> <mode={shifting, rounding}> <number of bits>\n";
		return 1;
	}

	return 0;
}

