#ifndef WAVQUANT_H
#define WAVQUANT_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>
#include <math.h>

class WAVQuant {
  private:
	std::vector<short> QuantizedSamples;
	std::vector<std::map<short, size_t>> counts;

  public:
	WAVQuant(const SndfileHandle& sfh) {
		counts.resize(sfh.channels());
	}

	std::vector<short> getQuantizedSamples(){
		return QuantizedSamples;
	}

	void update() {
		size_t n { };
		for(auto s : QuantizedSamples){
			counts[n++ % counts.size()][s]++;
		}
	}

	void quantize(const std::vector<short>& samples, int nbits) {
		//int delta = 65536 / pow(2, nbits);
		for(short s : samples){
			QuantizedSamples.push_back((s >> (16-nbits)) << (16-nbits));	
		}
		update();
	}

	void dumpSamples() const {
		for(auto value : QuantizedSamples)
			std::cout << value  << '\n';
	}


	void dumpRS(const size_t channel) const {
		for(auto [value, counter] : counts[channel])
			std::cout << value << '\t' << counter << '\n';
	}


};

#endif

