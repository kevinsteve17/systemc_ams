#include <systemc.h>
#include <stdlib.h>
#include "instRegDec.h"

int sc_main (int argc, char* argv[])
{
    sc_signal< int > instruction;

    sc_signal<sc_int<3>> sel;
    sc_signal< sc_int<32> > amplitude;
    sc_signal< sc_int<3> > offset;
    sc_signal< sc_int<4> > freq1;
    sc_signal< sc_int<3> > freq2;
    sc_signal< sc_int<8> > phase;

    instRegDec regDec("REGDEC");
        regDec.instruction(instruction);
        regDec.sel(sel);
        regDec.amplitude(amplitude);
        regDec.offset(offset);
        regDec.freq1(freq1);
        regDec.freq2(freq2);
        regDec.phase(phase);

    // Open VCD file
    sc_trace_file *wf = sc_create_vcd_trace_file("fsm");
    wf->set_time_unit(1, SC_MS);

    regDec.tracing(wf);

    cout << "@" << sc_time_stamp() <<" Initiating simulation\n" << endl;

    // instruction init value
    instruction = 0;

    sc_start(10,SC_MS);
    instruction = 25825; //-> 3 + 4*sin(2k)

    sc_start(40,SC_MS);

    cout << "@" << sc_time_stamp() <<" Terminating simulation\n" << endl;
    sc_close_vcd_trace_file(wf);
  
  return 0;                                       // Terminate simulation
 }