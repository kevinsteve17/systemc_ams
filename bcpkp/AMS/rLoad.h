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

#ifndef RLOAD_H_
#define RLOAD_H_

#include <systemc-ams>

SC_MODULE(rload)
{
    sca_eln::sca_terminal in, out;
    sca_eln::sca_r r1, r2;

    SC_CTOR(rload):in("in"),out("out"), gnd("gnd"), r1("r1", 100), r2("r2", 100)
    {
        r1.p(in);
        r1.n(out);
        r2.p(out);
        r2.n(gnd);
    }

    private:
	    sca_eln::sca_node_ref gnd;
};

#endif