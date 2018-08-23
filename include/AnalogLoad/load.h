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

#ifndef LOAD_H_
#define LOAD_H_

#include <systemc-ams>

SC_MODULE(load)
{
    sca_eln::sca_terminal in, out;
    sca_eln::sca_r r1;
    sca_eln::sca_r r2;
    //sca_eln::sca_c c1;

    SC_CTOR(load):in("in"),
                  out("out"), 
                  gnd("gnd"), 
                  r1("r1", 100), 
                  r2("r2", 100) 
                  //c1("c1", 0.0000001)
    {
        r1.p(in);
        r1.n(out);
        r2.p(out);
        r2.n(gnd);
        //c1.p(out);
        //c1.n(gnd);        
    }

    private:
	    sca_eln::sca_node_ref gnd;
};

#endif