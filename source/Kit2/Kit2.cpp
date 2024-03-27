#include "daisy_patch.h"
#include "daisysp.h"
#include <string>
#include "Utility.h"
#include "IDrum.h"
#include "Bd8.h"
#include "Sd8.h"
#include "SdNoise.h"

using namespace daisy;
using namespace daisysp;

#define MINIMUM_NOTE 36

DaisyPatch hw;
IDrum *drums[16];
uint8_t drumCount = 2;
Bd8 bd;
SdNoise rs;
Sd8 sd;
uint8_t currentDrum = 0;

void OledMessage(std::string message, int row) 
{
    char* mstr = &message[0];
    hw.display.SetCursor(0, row * 10);
    hw.display.WriteString(mstr, Font_6x8, true);
    hw.display.Update();
}

void OledMessage(std::string message, int row, int column) 
{
    char* mstr = &message[0];
    hw.display.SetCursor(column * 8, row * 10);
    hw.display.WriteString(mstr, Font_6x8, true);
    hw.display.Update();
}

void ProcessKnobs() {

    for (int knob = 0; knob < 4; knob++) {
        float sig = hw.controls[knob].Process();
        uint8_t param = 0;
        if (currentDrum == 0) {
            switch (knob) {
                case 0: param = Bd8::PARAM_FREQUENCY; break;
                case 1: param = Bd8::PARAM_AMP_DECAY; break;
                case 2: param = Bd8::PARAM_PITCH_DECAY; break;
                case 3: param = Bd8::PARAM_MOD_AMT; break;
            }
        } else if (currentDrum == 1) {
            switch (knob) {
                case 0: param = SdNoise::PARAM_ATTACK; break;
                case 1: param = SdNoise::PARAM_DECAY; break;
                case 2: param = SdNoise::PARAM_CURVE; break;
            }
        }
        drums[currentDrum]->SetParam(param, sig, true);
    }
}

void DisplayKnobs() {

    hw.display.DrawRect(0, 0, 127, 16, false, true);
    for (int knob = 0; knob < 4; knob++) {
        Rectangle rect(knob * 32, 0, 32, 8);
        uint8_t param = 0;
        if (currentDrum == 0) {
            switch (knob) {
                case 0: param = Bd8::PARAM_FREQUENCY; break;
                case 1: param = Bd8::PARAM_AMP_DECAY; break;
                case 2: param = Bd8::PARAM_PITCH_DECAY; break;
                case 3: param = Bd8::PARAM_MOD_AMT; break;
            }
        } else if (currentDrum == 1) {
            switch (knob) {
                case 0: param = SdNoise::PARAM_ATTACK; break;
                case 1: param = SdNoise::PARAM_DECAY; break;
                case 2: param = SdNoise::PARAM_CURVE; break;
            }
        }
        std::string sc = drums[currentDrum]->GetParamString(param);
        hw.display.WriteStringAligned(sc.c_str(), Font_6x8, rect, Alignment::centered, true);
    }
    hw.display.Update();
}

void ProcessControls()
{
    hw.ProcessAnalogControls();
    // hw.ProcessDigitalControls();

    // ProcessEncoder();
    ProcessKnobs();
    // ProcessGates();
}


void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{

    ProcessControls();

    for(size_t i = 0; i < size; i++)
    {

        float sig = 0.0f;
        for (uint8_t i = 0; i < drumCount; i++) {
            sig += drums[i]->Process();
        }
        float limited = sig / drumCount;

        out[0][i] = out[1][i] = limited;
        out[2][i] = out[3][i] = sig;

    }
}


// Typical Switch case for Message Type.
void HandleMidiMessage(MidiEvent m)
{
    switch(m.type)
    {
        case NoteOn:
        {
            // NoteOnEvent p = m.AsNoteOn();
            // This is to avoid Max/MSP Note outs for now..
            // if(m.data[1] != 0)
            // {
            //     p = m.AsNoteOn();
            //     osc.SetFreq(mtof(p.note));
            //     osc.SetAmp((p.velocity / 127.0f));
            // } else {
            //     osc.SetAmp(0.0f);
            // }
            // OledMessage("Midi: " + std::to_string(p.note) + ", " + std::to_string(p.velocity) + "     ", 3);

            NoteOnEvent p = m.AsNoteOn();
            float velocity = p.velocity / 127.0f;
            if (p.velocity > 0) {
                if (p.note >= MINIMUM_NOTE && p.note < MINIMUM_NOTE + drumCount) {
                    drums[p.note - MINIMUM_NOTE]->Trigger(velocity);
                }
            }
        }
        break;
        case NoteOff:
        {
        }
        break;
        case ControlChange:
        {
            ControlChangeEvent p = m.AsControlChange();
            switch(p.control_number)
            {
                case 1:
                    break;
                case 2:
                    break;
                default: break;
            }
            break;
        }
        default: break;
    }
}


// Main -- Init, and Midi Handling
int main(void)
{
    // Init
    float samplerate;
    hw.Init();
    samplerate = hw.AudioSampleRate();

    drums[0] = &bd;
    drums[1] = &rs;
    // drums[2] = &sd;
    drumCount = 2;
    currentDrum = 1;

    for (uint8_t i = 0; i < drumCount; i++) {
        drums[i]->Init(samplerate);
    }

    //display
    hw.display.Fill(false);
    OledMessage("Hachikit 0.1", 5);
    // Utility::DrawDrums(&hw.display, 5);
    // hw.display.Update();

    // Start stuff.
    hw.midi.StartReceive();
    hw.StartAdc();
    hw.StartAudio(AudioCallback);
    for(;;)
    {
        hw.midi.Listen();
        // Handle MIDI Events
        while(hw.midi.HasEvents())
        {
            HandleMidiMessage(hw.midi.PopEvent());
        }
        DisplayKnobs();
    }
}
