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
    sca_tdf::sca_out<double> out; // output port
    int amplitude;


    /*
    signalGen( sc_core::sc_module_name nm, int signal_type_ = 0,double ampl_= 1.0, double freq_ = 1.0e3, 
            double offset_ = 0.0, double phase_ = 1.0,
            sca_core::sca_time Tm_ = sca_core::sca_time(0.125, sc_core::SC_MS))
        : out("out"), signal_type(signal_type_), ampl(ampl_), freq(freq_), Tm(Tm_), offset(offset_), phase(phase_) {}
    */

   SCA_CTOR(signalGen)
   :out("out"), signal_type(0), ampl(1.0), freq(10e3), Tm(sca_core::sca_time(0.125, sc_core::SC_MS)), offset(0.0), phase(1.0) {}

    void set_attributes(sc_core::sc_module_name nm, int signal_type_ = 0,double ampl_= 1.0, double freq_ = 1.0e3, 
            double offset_ = 0.0, double phase_ = 1.0,
            sca_core::sca_time Tm_ = sca_core::sca_time(0.125, sc_core::SC_MS));
    
    void processing();

    private:
        int signal_type;            // 0: sin, 1: square, 2: triangle, 3: sawtooth
        double ampl;                // amplitude
        double freq;                // frequency
        sca_core::sca_time Tm;      // module time step
        double offset;              // offset
        double phase;               // phase


};

#endif