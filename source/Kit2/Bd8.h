#ifndef BD8_H
#define BD8_H

#include "daisy_patch.h"
#include "daisysp.h"
#include <string>
#include "IDrum.h"
#include "Utility.h"

using namespace daisy;
using namespace daisysp;

class Bd8: public IDrum {

    public:
        // Number of settable parameters for this model.
        static const uint8_t PARAM_COUNT = 6;
        static const uint8_t PARAM_FREQUENCY = 0;
        static const uint8_t PARAM_AMP_ATTACK = 1;
        static const uint8_t PARAM_AMP_DECAY = 2;
        static const uint8_t PARAM_PITCH_ATTACK = 3;
        static const uint8_t PARAM_PITCH_DECAY = 4;
        static const uint8_t PARAM_MOD_AMT = 5;

        /** Initialize model with default parameters.
         * \param sample_rate audio sample rate.
        */
        void Init(float sample_rate);

        /** Initialize model with specified parameters.
         * \param sample_rate audio sample rate.
         * \param frequency oscillator frequency in hertz.
        */
        void Init(float sample_rate, float frequency, float ampAttack, float ampDecay, float pitchAttack, float pitchDecay, float modAmount);

        float Process();
        void Trigger(float velocity);

        /** Get the current value of a parameter.
         * \param param index of the desired parameter (must be <PARAM_COUNT).
        */
        float GetParam(uint8_t param);

        /** Get the current value of a parameter as a readable string.
         * \param param index of the desired parameter (must be <PARAM_COUNT).
        */
        std::string GetParamString(uint8_t param);

        /** Set the value of a parameter.
         * \param param index of the desired parameter (must be <PARAM_COUNT).
         * \param value
         * \param scaled when true, value is in the range 0-1.
        */
        float SetParam(uint8_t param, float value, bool scaled);

        std::string Name() { return "Bd8"; }
        std::string Slot() { return slot; }
        std::string ParamName(uint8_t param) { return param < PARAM_COUNT ? paramNames[param] : ""; }

    private:
        std::string paramNames[PARAM_COUNT] = { "Freq", "aAtt", "aDec", "pAtt", "pDec", "Mod" };
        std::string slot;
        Oscillator osc;
        AdEnv ampEnv;
        AdEnv pitchEnv;
        float velocity;
        float params[PARAM_COUNT];

    public:
        // /** Set the base frequency of the sine wave in hertz.
        //  * \param frequency oscillator frequency in hertz.
        // */
        // void SetFrequency(float frequency) { 
        //     this->frequency = Utility::LimitFloat(frequency, 20, 5000); 
        //     osc.SetFreq(this->frequency);
        // }
        // float GetFrequency() { return frequency; }

        // /** Set the attack time for the amp envelope, in seconds.
        //  * \param ampAttack attack time in seconds.
        // */
        // void SetAmpAttack(float ampAttack) { 
        //     this->ampAttack = Utility::LimitFloat(ampAttack, 0, MAX_ENV_TIME); 
        //     ampEnv.SetTime(ADENV_SEG_ATTACK, this->ampAttack);
        // }
        // float GetAmpAttack() { return ampAttack; }

        // /** Set the decay time for the amp envelope, in seconds.
        //  * \param ampDecay decay time in seconds.
        // */
        // void SetAmpDecay(float ampDecay) { 
        //     this->ampDecay = Utility::LimitFloat(ampDecay, 0, MAX_ENV_TIME); 
        //     ampEnv.SetTime(ADENV_SEG_DECAY, this->ampDecay);
        // }
        // float GetAmpDecay() { return ampDecay; }

        // /** Set the attack time for the pitch envelope, in seconds.
        //  * \param pitchAttack attack time in seconds.
        // */
        // void SetPitchAttack(float pitchAttack) { 
        //     this->pitchAttack = Utility::LimitFloat(pitchAttack, 0, MAX_ENV_TIME); 
        //     pitchEnv.SetTime(ADENV_SEG_ATTACK, this->pitchAttack);
        // }
        // float GetPitchAttack() { return pitchAttack; }

        // /** Set the decay time for the pitch envelope, in seconds.
        //  * \param pitchDecay decay time in seconds.
        // */
        // void SetPitchDecay(float pitchDecay) { 
        //     this->pitchDecay = Utility::LimitFloat(pitchDecay, 0, MAX_ENV_TIME); 
        //     pitchEnv.SetTime(ADENV_SEG_DECAY, this->pitchDecay);
        // }
        // float GetPitchDecay() { return pitchDecay; }

        // /** Set the modulation amount for the pitch envelope, in hertz.
        //  * \param modAmount maximum modulation in hertz.
        // */
        // void SetModAmount(float modAmount) { this->modAmount = modAmount; }
        // float GetModAmount() { return modAmount; }

};



#endif
