#ifndef WAVQUANT_H
#define WAVQUANT_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>
#include <math.h>

class WAVQuant {
  private:
	std::vector<short> samples;
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
			this->samples.push_back(s);
			QuantizedSamples.push_back((s >> (16-nbits)) << (16-nbits));	
		}
		update();
	}


    std::vector<short> quantize(short numBits) {
        std::vector<short> res(samples.size());
        int delta = (int) pow(2, 16 - numBits);
        for (unsigned long i = 0; i < samples.size(); i++) {
            if (samples[i] % delta >= delta / 2) {
                res[i] = delta*(1+samples[i]/delta);
            } else {
                res[i] = samples[i] - (samples[i] % delta);
            }
        }
        return res;
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

