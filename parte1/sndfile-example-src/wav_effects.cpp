#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_effects.h"


using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {

	if(argc < 2) {
		cerr << "Usage: " << argv[0] << " <original file>\n";
		return 1;
	}

	SndfileHandle sndFile_original { argv[argc-1] };


	if(sndFile_original.error()) {
		cerr << "Error: invalid original file\n";
		return 1;
    }

	if((sndFile_original.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: original file is not in WAV format\n";
		return 1;
	}

	if((sndFile_original.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: original file is not in PCM_16 format\n";
		return 1;
	}


	size_t nFrames;

	vector<short> original_samples(FRAMES_BUFFER_SIZE * sndFile_original.channels());
	
	WAVEffects eff {sndFile_original};
	while((nFrames = sndFile_original.readf(original_samples.data(), FRAMES_BUFFER_SIZE))) {
		original_samples.resize(nFrames * sndFile_original.channels());
		eff.update(original_samples);
	}

	short k = 4410;
	double alpha = 0.8;
	eff.ApplySingleEcho(k, alpha); 
	eff.ApplyMultipleEchoes(k,alpha);

	vector<short> single_echo_samples = eff.GetSingleEchoSamples();
	vector<short> multiple_echo_samples = eff.GetMultipleEchoSamples();

	SndfileHandle SEFile { "SingleEcho.wav", SFM_WRITE, sndFile_original.format(), sndFile_original.channels(),sndFile_original.samplerate()};
	SndfileHandle MEFile { "MultipleEcho.wav", SFM_WRITE, sndFile_original.format(), sndFile_original.channels(),sndFile_original.samplerate()};
	
	SEFile.writef(single_echo_samples.data(), sndFile_original.frames()+k);
	MEFile.writef(multiple_echo_samples.data(), sndFile_original.frames()+1);




	return 0;
}