#ifndef INSTREGDEC_H
#define INSTREGDEC_H

#include <systemc.h>
#include <stdlib.h>

SC_MODULE(instRegDec)
{
    // port definition
    sc_in<int> instruction;

    sc_out<sc_int<3>> sel;
    sc_out<sc_int<3>> amplitude;
    sc_out<sc_int<3>> offset;
    sc_out<sc_int<4>> freq1;
    sc_out<sc_int<3>> freq2;
    sc_out<sc_int<8>> phase;

    // methods
    void decodeWaveSelector();
    void decodeAmplitude();
    void decodeOffset();
    void decodeFreq();
    void decodeFreqMultiplier();
    void decodePhase();
    void tracing(sc_trace_file *tf);

    // constructor
    SC_CTOR(instRegDec)
    {
        SC_METHOD(decodeWaveSelector);
            sensitive << instruction;
        
        SC_METHOD(decodeAmplitude);
            sensitive << instruction;

        SC_METHOD(decodeOffset);
            sensitive << instruction;

        SC_METHOD(decodeFreq);
            sensitive << instruction;

        SC_METHOD(decodeFreqMultiplier);
            sensitive << instruction;

        SC_METHOD(decodePhase);
            sensitive << instruction;
    }
};
#endif