#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_quant.h"


using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {

	if(argc < 2) {
		cerr << "Usage: " << argv[0] << " <input file>\n";
		return 1;
	}

	SndfileHandle sndFile { argv[argc-1] };

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

	short nbits=15;
	
	WAVQuant quant { sndFile };
	while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * sndFile.channels());
		quant.quantize(samples,nbits); //reduce to x bits
		
	}

	
	vector<short> QuantizedSamples = quant.getQuantizedSamples();
	outFile.writef(QuantizedSamples.data(), sndFile.frames());


	// DIOGO
	// outFile.writef(quant.quantize(nbits).data(), sndFile.frames());
	
	

	// quant.dumpRS(0);
	// quant.dumpSamples();


	return 0;
}

