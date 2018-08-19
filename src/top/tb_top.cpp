/*******************************************************
 *  Instituto Tecnologico de Costa Rica
 *
 *  Diseño de Alto Nivel de Sistema Electrónicos
 *
 *  Proyecto 3 (Grupo 1)
 *
 *  Mario Castro    200827325
 *  Diego Herrera   200324558
 *  Fernando París  200510153
 *  Esteban Rivera  2018319491
 *  Kevin Víquez    200944341
 *******************************************************/

#include "CpuTlmMem/tlmNet.h"
#include "InstRegisterDeco/instRegDec.h"

int sc_main(int argc, char* argv[])   
{
    // wave gen instruction (from MMIO space) signal
    sc_signal<int> wavegen_instruction;

    // Instruction register decoder signals
    sc_signal<sc_int<3>> sel;
    sc_signal< sc_int<3> > amplitude;
    sc_signal< sc_int<3> > offset;
    sc_signal< sc_int<4> > freq1;
    sc_signal< sc_int<3> > freq2;
    sc_signal< sc_int<8> > phase;    

    // CPU<->TLM<->MEM net instance
    TlmNet tlmnet("tlmnet");
    tlmnet.mmio_wavegen_sector_output(wavegen_instruction);
 
    // Instruction register instancec
    instRegDec regDec("REGDEC");
        regDec.instruction(wavegen_instruction);
        regDec.sel(sel);
        regDec.amplitude(amplitude);
        regDec.offset(offset);
        regDec.freq1(freq1);
        regDec.freq2(freq2);
        regDec.phase(phase);    

    // Open and set VCD file
    sc_trace_file *wf = sc_create_vcd_trace_file("CPU_TLM_MEM");
    wf->set_time_unit(1, SC_NS);

    regDec.tracing(wf);
    sc_trace(wf, wavegen_instruction, "wavegen_instruction");

    sc_start(3000,SC_MS);

    cout << "@" << sc_time_stamp() <<" Terminating simulation\n" << endl;
    sc_close_vcd_trace_file(wf);
    return 0;
}
