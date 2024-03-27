#ifndef SD8_H
#define SD8_H

#include "daisy_patch.h"
#include "daisysp.h"
#include <string>
#include "IDrum.h"

using namespace daisy;
using namespace daisysp;


// class Sd8: public IDrum {
class Sd8 {

    public:
        void Init(float sample_rate);
        float Process();
        void Trigger(float velocity);
        std::string Name();

    private:
        Oscillator osc;
        AdEnv oEnv;
        WhiteNoise noise;
        AdEnv nEnv;
        float velocity;

};



#endif
