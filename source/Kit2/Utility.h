#ifndef UTILITY_H
#define UTILITY_H

#include "daisy_patch.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

#define MAX_ENV_TIME 20

class Utility {

    public:
        static float Limit(float value) {
            return std::min(1.0f, std::max(0.0f, value));
        }

        static float LimitFloat(float value, float min, float max) {
            return std::min(max, std::max(min, value));
        }

        static float LimitInt(int value, int min, int max) {
            return std::min(max, std::max(min, value));
        }

        static float ScaleFloat(float value, int min, int max, Parameter::Curve curve) {

            float scaled = value;
            switch(curve)
            {
                case Parameter::LINEAR: 
                    scaled = (value * (max - min)) + min; 
                    break;
                case Parameter::EXPONENTIAL:
                    scaled = ((value * value) * (max - min)) + min;
                    break;
                case Parameter::LOGARITHMIC:
                    scaled = expf((value * (max - min)) + min);
                    break;
                case Parameter::CUBE:
                    scaled = ((value * (value * value)) * (max - min)) + min;
                    break;
                default: break;
            }
            return scaled;
        }

        static void DrawDrums(daisy::OledDisplay<daisy::SSD130x4WireSpi128x64Driver> *display, uint8_t current) {
            display->DrawRect(0, 48, 127, 63, true, false);
            display->SetCursor(8, 52);
            display->WriteString("BD", Font_6x8, true);
            display->SetCursor(104, 52);
            display->WriteString("36", Font_6x8, true);

            for (uint8_t i = 0; i < 16; i++) {
                uint8_t x = 32 + (i % 8) * 8;
                uint8_t y = 48 + (i / 8) * 8;
                bool fill = i == current;
                display->DrawRect(x, y, x+7, y + 7, true, fill);
                if (fill) {
                    display->DrawRect(x, y, x+7, y + 7, false, false);
                }
            }

        }
};


#endif
