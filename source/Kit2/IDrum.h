#ifndef IDRUM_H
#define IDRUM_H

using namespace daisy;
using namespace daisysp;

#include "daisy_patch.h"
#include "daisysp.h"
#include <string>


class IDrum {

    public:
        virtual void Init(float sample_rate) = 0;
        virtual float Process() = 0;
        virtual void Trigger(float velocity) = 0;
        virtual float GetParam(uint8_t param) = 0;
        virtual float SetParam(uint8_t param, float value, bool scaled) = 0;

        virtual std::string Name() = 0;
        virtual std::string Slot() = 0;
        virtual std::string ParamName(uint8_t param) = 0;
        virtual std::string GetParamString(uint8_t param) = 0;

};


#endif
