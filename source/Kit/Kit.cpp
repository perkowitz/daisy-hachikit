#include "daisy_patch.h"
#include "daisysp.h"
#include <string>

using namespace daisy;
using namespace daisysp;

DaisyPatch hw;
// AnalogBassDrum kick;
// AnalogSnareDrum snare;
Oscillator osc1;
Oscillator osc2;

void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    // float mix;
    for(size_t i = 0; i < size; i++)
    {
        // mix = 0.5 * kick.Process();
        // mix += 0.5 * snare.Process();
        // for(size_t chn = 0; chn < 4; chn++)
        // {
        //     out[chn][i] = mix;
        // }
    }
}


void OledMessage(std::string message) 
{
    char* mstr = &message[0];
    hw.display.SetCursor(0, 30);
    hw.display.WriteString(mstr, Font_7x10, true);
    hw.display.Update();
}



// Typical Switch case for Message Type.
void HandleMidiMessage(MidiEvent m)
{
    OledMessage("midi stuff");
    switch(m.type)
    {
        case NoteOn:
        {
            NoteOnEvent p = m.AsNoteOn();
            // This is to avoid Max/MSP Note outs for now..
            if(m.data[1] != 0)
            {
                p = m.AsNoteOn();

                if(m.data[1] != 0)
                {
                    p = m.AsNoteOn();
                    osc2.SetFreq(mtof(p.note));
                    osc2.SetAmp((p.velocity / 127.0f));
                } else {
                    osc2.SetAmp(0.0f);
                }


                switch (p.note) {
                    case 36:
                        // OledMessage("Note 36");
                        // kick.Trig();
                        if(m.data[1] != 0)
                        {
                            p = m.AsNoteOn();
                            osc2.SetAmp((p.velocity / 127.0f));
                        } else {
                            osc2.SetAmp(0.0f);
                        }

                        break;
                    case 38:
                        // OledMessage("Note 38");
                        // snare.Trig();
                        break;
                }

            }
            OledMessage("NoteOn");
        }
        break;
        case NoteOff:
        {
            osc2.SetAmp(0.0f);
            OledMessage("NoteOff");
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

    // Instruments
    // kick.Init(samplerate);
    // kick.SetFreq(40);
    // kick.SetTone(0.5);
    // kick.SetDecay(0.4);
    // kick.SetAttackFmAmount(0.2);
    // kick.SetSelfFmAmount(0.3);
    // snare.Init(samplerate);
    // snare.SetDecay(300);
    // snare.SetSnappy(0.5);
    // snare.SetTone(0.5);

    osc1.Init(samplerate);
    osc1.SetWaveform(Oscillator::WAVE_SIN);
    osc1.SetFreq(80);

    osc2.Init(samplerate);
    osc2.SetWaveform(Oscillator::WAVE_SAW);
    osc2.SetFreq(440);

    //display
    // UpdateOled();
    std::string str  = "Kit";
    char*       cstr = &str[0];
    hw.display.WriteString(cstr, Font_7x10, true);
    hw.display.Update();
    OledMessage("Starting");

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
    }
}
