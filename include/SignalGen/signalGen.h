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

#ifndef SINE_H_
#define SINE_H_

#include "stdio.h"
#include "systemc.h"
#include "systemc-ams.h"
#include "math.h"


SCA_TDF_MODULE (signalGen) 
{
    sca_tdf::sca_de::sca_in<int> ampl;
    sca_tdf::sca_de::sca_in<int> signal_type;
    sca_tdf::sca_de::sca_in<int> offset;
    sca_tdf::sca_de::sca_in<int> phase;
    sca_tdf::sca_de::sca_in<int> frequency;
    sca_tdf::sca_de::sca_in<int> freq_multiplier;

    sca_tdf::sca_out<double> out; // output port 

   SCA_CTOR(signalGen)
   :out("out"), Tm(sca_core::sca_time(0.125, sc_core::SC_MS)) {}

    void set_attributes(sc_core::sc_module_name nm,
                        sca_core::sca_time Tm_ = sca_core::sca_time(0.125, sc_core::SC_MS));
    
    void processing();

    private:
        sca_core::sca_time Tm;      // module time step
};

#endif