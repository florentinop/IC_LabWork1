//
// Created by diogo on 03/10/22.
//

#ifndef SNDFILE_EXAMPLE_WAV_QUANT_H
#define SNDFILE_EXAMPLE_WAV_QUANT_H


#include <utility>
#include <cmath>

class WAVQuant {
private:
    std::vector<short> samples;

public:
    explicit WAVQuant(std::vector<short>& samples) {
        this->samples = samples;
    }

    std::vector<short> quantize(short numBits) {
        std::vector<short> res(this->samples.size());
        int delta = (int) pow(2, 16 - numBits);
        for (unsigned long i = 0; i < samples.size(); i++) {
            if (samples[i] % delta >= delta / 2) {
                res[i] = samples[i] + (samples[i] % delta);
            } else {
                res[i] = samples[i] - (samples[i] % delta);
            }
        }
        return res;
    }
};


#endif //SNDFILE_EXAMPLE_WAV_QUANT_H
