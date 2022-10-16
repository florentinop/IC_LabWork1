#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>


class WAVHist {
  private:
	std::vector<std::map<short, size_t>> counts;
	std::map<short, size_t> countsMID;
	std::map<short, size_t> countsSIDE;

  public:
	WAVHist(const SndfileHandle& sfh) {
		counts.resize(sfh.channels());
	}

	void update(const std::vector<short>& samples) {
		size_t n { };
		for(auto s : samples){
			counts[n++ % counts.size()][s]++;
		}

		/* for(size_t i=0; i < samples.size(); i++){
			counts[i % counts.size()][samples[i]]++;
		} */
	}

	void updateMID(const std::vector<short>& samples) {
		for(size_t i=0; i < samples.size(); i=i+2){
			countsMID[(samples[i]+samples[i+1])/2]++;
		}
	}

	void updateSIDE(const std::vector<short>& samples) {
		for(size_t i=0; i < samples.size(); i=i+2){
			countsSIDE[((int)samples[i]-(int)samples[i+1])/2]++;
		}
	}

	/* void dumpMID() {

		auto left = counts[0].begin();
		auto right = counts[1].begin();

		while (left != counts[0].end())
		{
			std::cout << left->first << '\t' << (left->second + right->second)/2 <<'\n';
			left++;
			right++;
		}
	}

	void dumpSIDE() {

		auto left = counts[0].begin();
		auto right = counts[1].begin();

		while (left != counts[0].end())
		{
			std::cout << left->first << '\t' << ((int)left->second - (int)right->second)/2 <<'\n';
			left++;
			right++;
		}
	} */

	void dump(const size_t channel) const {
		for(auto [value, counter] : counts[channel])
			std::cout << value << '\t' << counter << '\n';
	}

	void dumpMID() const {
		for(auto [value, counter] : countsMID)
			std::cout << value << '\t' << counter << '\n';
	}

	void dumpSIDE() const {
		for(auto [value, counter] : countsSIDE)
			std::cout << value << '\t' << counter << '\n';
	}


};

#endif

