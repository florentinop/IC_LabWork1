#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_cmp.h"


using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {

	if(argc < 3) {
		cerr << "Usage: " << argv[0] << " <original file> <modified file>\n";
		return 1;
	}

	SndfileHandle sndFile_original { argv[argc-2] };
	SndfileHandle sndFile_mod { argv[argc-1] };

	if(sndFile_original.error()) {
		cerr << "Error: invalid original file\n";
		return 1;
    }

	if(sndFile_mod.error()) {
		cerr << "Error: invalid modified file\n";
		return 1;
    }

	if((sndFile_original.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: original file is not in WAV format\n";
		return 1;
	}

	if((sndFile_mod.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: modified file is not in WAV format\n";
		return 1;
	}

	if((sndFile_original.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: original file is not in PCM_16 format\n";
		return 1;
	}

	if((sndFile_mod.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: modified file is not in PCM_16 format\n";
		return 1;
	}

	size_t nFrames;

	vector<short> original_samples(FRAMES_BUFFER_SIZE * sndFile_original.channels());
	vector<short> mod_samples(FRAMES_BUFFER_SIZE * sndFile_mod.channels());
	
	WAVCmp cmp {};
	while((nFrames = sndFile_original.readf(original_samples.data(), FRAMES_BUFFER_SIZE))) {
		original_samples.resize(nFrames * sndFile_original.channels());
		cmp.update(original_samples,1);
	}

	while((nFrames = sndFile_mod.readf(mod_samples.data(), FRAMES_BUFFER_SIZE))) {
		mod_samples.resize(nFrames * sndFile_mod.channels());
		cmp.update(mod_samples,0);
	}

	
	cmp.computeSNR();
	cmp.dumpSNR();
	// cmp.dumpNoise();


	return 0;
}

