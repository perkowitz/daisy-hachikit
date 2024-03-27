#include "Sd8.h"
#include "Utility.h"

using namespace daisy;
using namespace daisysp;

void Sd8::Init(float sample_rate) {
    osc.Init(sample_rate);
    oEnv.Init(sample_rate);
    noise.Init();
    nEnv.Init(sample_rate);

    osc.SetFreq(220);
    osc.SetWaveform(Oscillator::WAVE_SIN);
    oEnv.Init(sample_rate);
    oEnv.SetMax(1);
    oEnv.SetMin(0);
    oEnv.SetCurve(-20);
    oEnv.SetTime(ADENV_SEG_ATTACK, 0.001);
    oEnv.SetTime(ADENV_SEG_DECAY, 0.5);

    nEnv.Init(sample_rate);
    nEnv.SetMax(1);
    nEnv.SetMin(0);
    nEnv.SetCurve(0-20);
    nEnv.SetTime(ADENV_SEG_ATTACK, 0.001);
    nEnv.SetTime(ADENV_SEG_DECAY, 0.3);

}

float Sd8::Process() {
    return velocity * (osc.Process() * oEnv.Process() + noise.Process() * nEnv.Process()) / 2;
}

void Sd8::Trigger(float velocity) {
    this->velocity = Utility::Limit(velocity);
    if (this->velocity > 0) {
        oEnv.Trigger();
        nEnv.Trigger();
    }
}

std::string Sd8::Name() {
    return "Sd8";
}