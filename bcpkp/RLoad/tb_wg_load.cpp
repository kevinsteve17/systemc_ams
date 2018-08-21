#include <systemc-ams>
#include "sine.h"
#include "rLoad.h"

int sc_main (int argc, char* argv[])
{
    sca_eln::sca_node sigInput;
	sca_eln::sca_node sigOutput;
    sca_tdf::sca_signal<double> vp;
	sca_tdf::sca_signal<double> a;
	sca_tdf::sca_signal<bool> b;
	sca_eln::sca_node_ref gnd;

	sca_eln::sca_tdf_vsource *src;
    src = new sca_eln::sca_tdf_vsource("src",1.0);
	
    sc_core::sc_time time_step(10.0, sc_core::SC_US);

    sine sin0("sin0",3.3,1000,time_step);

    sin0.out(vp);
	sin0.aux = 3;
    src->inp(vp);

	src->p(sigInput);
	src->n(gnd);

    rload res_circuit("circuit");

	res_circuit.in(sigInput);
	res_circuit.out(sigOutput);

	sca_util::sca_trace_file *eln= sca_util::sca_create_vcd_trace_file("out.vcd");
	sca_trace(eln, sigInput, "signalInput");
	sca_trace(eln, sigOutput, "Output");

	sc_start(5, sc_core::SC_MS);
	//b = false;
	sin0.aux = 1;
	sc_start(2,SC_MS);
	
	sca_util::sca_close_vcd_trace_file(eln);



	return 0;   
}