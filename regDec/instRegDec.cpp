#include "instRegDec.h"

void instRegDec::decodeWaveSelector()
{
    sel = (instruction & 0x7);
}

void instRegDec::decodeAmplitude()
{
    amplitude = (instruction & 0x38) >> 3;
}

void instRegDec::decodeOffset()
{
    offset = (instruction & 0x1C0) >> 6;
}

void instRegDec::decodeFreq()
{
    freq1 = (instruction & 0x1E00) >> 9;
}

void instRegDec::decodeFreqMultiplier()
{
    freq2 = (instruction & 0xE000) >> 13;
}

void instRegDec::decodePhase()
{
    phase = (instruction & 0xFF0000) >> 16;
}

void instRegDec::tracing(sc_trace_file *tf)
{
    const std::string str = this->name();

    // Dump local signals
    sc_trace(tf, this->instruction, str+".instruction");
    sc_trace(tf, this->sel, str+".sel");
    sc_trace(tf, this->amplitude, str+".amplitude");
    sc_trace(tf, this->offset, str+".offset");
    sc_trace(tf, this->freq1, str+".freq1");
    sc_trace(tf, this->freq2, str+".freq2");
    sc_trace(tf, this->phase, str+".phase");    
}