#ifndef WAVQUANT_H
#define WAVQUANT_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>
#include <math.h>

class WAVCmp {
  private:
  std::vector<short> original_samples;
  std::vector<short> mod_samples;
  std::vector<short> Noise;

  double NoiseEnergy;
  double SignalEnergy;

  short MaxError;
  double SNR;

  public:

	WAVCmp() {
		NoiseEnergy = 0.0;
		SignalEnergy = 0.0;
		SNR = 0.0;
		MaxError = 0;
	}

	void update(const std::vector<short>& samples, int original) {
		if(original==1){
			for(auto s : samples)
				original_samples.push_back(s); 
		}
		if(original==0){
			for(auto s : samples)
			mod_samples.push_back(s); 
		}
	}

	void ComputeNoise() {
		short Error = 0;
		Noise.resize(original_samples.size());
		for (size_t i = 0; i < Noise.size(); i++){
			Error = original_samples[i] - mod_samples[i]; 
			Noise[i] = Error;
			if(abs(Error) > MaxError){
				MaxError = abs(Error);
			}
			
		}
	}

	void ComputeNoiseEnergy() {
		for(short n : Noise){
			NoiseEnergy = NoiseEnergy + pow(abs(n),2);
		}
	}

	void ComputeSignalEnergy() {
		for(short x : original_samples){
			SignalEnergy = SignalEnergy + pow(abs(x),2);
		}
	}

	

	void computeSNR() {
		ComputeNoise();
		ComputeSignalEnergy();
		ComputeNoiseEnergy();
		if(NoiseEnergy==0){
			SNR=0;
			return;
		}
		SNR = 10*log10(SignalEnergy/NoiseEnergy);
		
		// std::cout  << "Signal Energy: " << SignalEnergy << "\tNoise Energy: " << NoiseEnergy << '\n';
	}

	void dumpNoise() const {
		for(short n : Noise){
			std::cout << n << '\n';
		}
	}

	void dumpSNR() const {
		std::cout << "SNR: " << SNR << " dB" << "\nMaximum per sample absolute error: " << MaxError <<'\n';
	}

	

	//void dumpSNR() 


};

#endif

