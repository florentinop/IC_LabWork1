#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_hist.h"
#include "wav_quant.h"


using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {

	if(argc < 4) {
		cerr << "Usage: " << argv[0] << " <input file> <channel> <mode> mode={normal,mid,side}\n";
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

	int channel { stoi(argv[argc-2]) };
	if(channel >= sndFile.channels()) {
		cerr << "Error: invalid channel requested\n";
		return 1;
	}

	size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
	
	WAVHist hist { sndFile };
	while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * sndFile.channels());
		hist.update(samples);
		hist.updateMID(samples);
		hist.updateSIDE(samples);
	}

	string mode = argv[3];

	if(mode=="normal")
		hist.dump(channel);
	else if(mode=="mid")
		hist.dumpMID();
	else if(mode=="side")
		hist.dumpSIDE();
	else{
		cerr << "Usage: " << argv[0] << " <input file> <channel> <mode={normal,mid,side}>\n";
		return 1;
	}
	return 0;
}

