
#include <systemc-ams>
#include "signalGen.h"
#include "rLoad.h"

int sc_main (int argc, char* argv[])

{
    uint32_t i;
    sca_tdf::sca_signal<double> vp;
    sca_tdf::sca_signal<double> vn;

    sca_eln::sca_tdf_vsource *vin;      // TDF -> V(t)
    sca_eln::sca_tdf_vsink *vo;         // V(t) -> TDF
    sca_eln::sca_node_ref gnd;

    sca_eln::sca_node sigInput;
    sca_eln::sca_node signalLoad;

    sc_core::sc_time time_step(10.0, sc_core::SC_NS);

    signalGen signalWave("signalWave");
    signalWave.set_attributes("signalWave",0,1.0,10000,0.0, 1.0, time_step);
    signalWave.out(vp);

    vin = new sca_eln::sca_tdf_vsource ("vin", 1.0);

    vin->inp(vp);
    vin->p(sigInput);
    vin->n(gnd);

    rload resLoad("R R");
    resLoad.in(sigInput);
    resLoad.out(signalLoad); 

    vo = new sca_eln::sca_tdf_vsink ("vout", 1.0);
    vo->p(signalLoad);
    vo->n(gnd);
    vo->outp(vn);

    
    sca_util::sca_trace_file *vcdfile= sca_util::sca_create_vcd_trace_file("signGen-tdf.vcd");
    sca_trace(vcdfile, vp, "Vin_tdf");
    sca_trace(vcdfile, sigInput, "Vin_eln");
    sca_trace(vcdfile, signalLoad, "signalLoad");

    sc_start(5, sc_core::SC_MS);

    sca_util::sca_close_vcd_trace_file(vcdfile);



	return 0;
}