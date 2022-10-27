#ifndef WAVEFFECTS_H
#define WAVEFFECTS_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>
#include <math.h>

class WAVEffects {
  private:
	std::vector<short> Samples;
	std::vector<short> SingleEchoSamples;
	std::vector<short> MultipleEchoSamples;
	std::vector<std::map<short, size_t>> counts;

  public:
	WAVEffects(const SndfileHandle& sfh) {
		counts.resize(sfh.channels());
	}


	void update(const std::vector<short>& samples) {
		for(short s : samples)		// save original samples
			Samples.push_back(s);
	}

	void ApplySingleEcho(short k, double alpha) {

		for(size_t i = 0; i < k*2; i++)
			SingleEchoSamples.push_back(0);	//apply delay

		for(short s : Samples)			//copy and reduce intensity by alpha
			SingleEchoSamples.push_back(s*alpha);

		for(size_t i = 0; i < k*2; i++)
			Samples.push_back(0);	// add space for the delay

		for(size_t i = 0; i<Samples.size(); i++)
			SingleEchoSamples[i] = (SingleEchoSamples[i] + Samples[i]) / (1+alpha);	//add signals and divide by 1+alpha so that we never saturate the signal
																	// 1+alpha is the worst case  
	}

	void ApplyMultipleEchoes(short k, double alpha) {
	
		for (int i = 3; i < Samples.size(); i=i+2){
			
			if(i-3-k < 0 || i-2-k<0){
				MultipleEchoSamples.push_back(Samples[i-3]);
				MultipleEchoSamples.push_back(Samples[i-2]);
				continue;
			}

			MultipleEchoSamples.push_back(((Samples[i-1] + alpha * MultipleEchoSamples[i-3-k]))/(1+ alpha)); // left channel
			MultipleEchoSamples.push_back(((Samples[i] + alpha * MultipleEchoSamples[i-2-k]))/(1+ alpha));	// right channel
		}

		for(size_t i = 0; i < k*2; i++)
			MultipleEchoSamples.push_back(0);

		
	}

	std::vector<short> GetSingleEchoSamples() {
		return SingleEchoSamples;
	}

	std::vector<short> GetMultipleEchoSamples() {
		return MultipleEchoSamples;
	}


	void dumpSamples() const {
		for(auto value : Samples)
			std::cout << value  << '\n';
	}


};

#endif