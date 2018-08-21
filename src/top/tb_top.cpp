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
#include "SignalGen/signalGen.h"
#include "AnalogLoad/load.h"
#include <systemc-ams>

int sc_main(int argc, char* argv[])   
{
    // wave gen instruction (from MMIO space) signal
    sc_signal<int> wavegen_instruction;

    // Instruction register decoder signals
    sc_signal<int> sel;
    sc_signal<int> amplitude;
    sc_signal<int> offset;
    sc_signal<int> freq1;
    sc_signal<int> freq2;
    sc_signal<int> phase;

    // signals wave generator
    sca_tdf::sca_signal<double> signal_gen_output;

    // load terminals and nodes
    sca_eln::sca_tdf_vsource *vin;      // TDF -> V(t)
    sca_eln::sca_node_ref gnd;    
    sca_eln::sca_node sigInput;
    sca_eln::sca_node vload;
    sca_tdf::sca_signal<double> v_out;    

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

    // Signal generator isntance
    signalGen signalWave("signalWave");
        sc_core::sc_time time_step(10.0, sc_core::SC_NS);
        signalWave.set_attributes("signalWave", time_step);
        signalWave.out(signal_gen_output);
        signalWave.offset(offset);
        signalWave.signal_type(sel);
        signalWave.ampl(amplitude);
        signalWave.phase(phase);
        signalWave.frequency(freq1);
        signalWave.freq_multiplier(freq2);

    // ELN module instance
    vin = new sca_eln::sca_tdf_vsource ("vin", 1.0);
    vin->inp(signal_gen_output);
    vin->p(sigInput);
    vin->n(gnd);

    load resLoad("resLoad");
        resLoad.in(sigInput);
        resLoad.out(vload);

    // Open and set VCD file
    sca_util::sca_trace_file *wf= sca_util::sca_create_vcd_trace_file("proyecto3.vcd");

    //regDec.tracing(wf); 
    sca_trace(wf, wavegen_instruction, "wavegen_instruction");
    sca_trace(wf, sel, "sel");
    sca_trace(wf, amplitude, "amplitude");
    sca_trace(wf, offset, "offset");
    sca_trace(wf, freq1, "frequency");
    sca_trace(wf, freq2, "freq_multiplier");    
    sca_trace(wf, signal_gen_output, "signal_gen_output");
    //sca_trace(wf, vload, "vload output signal");

    // start sim
    sc_start(17,SC_MS);

    cout << "@" << sc_time_stamp() <<" Terminating simulation\n" << endl;
    sca_util::sca_close_vcd_trace_file(wf);
    return 0;
}
