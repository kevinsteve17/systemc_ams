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

#include "signalGen.h"
#include "math.h"

 
    void signalGen::set_attributes(sc_core::sc_module_name nm, int signal_type_, double ampl_, double freq_, double offset_, double phase_ ,sca_core::sca_time tm) {
        set_timestep(tm);
        signal_type = signal_type_; 
        
    }
    
    void signalGen::processing() {
        double t = get_time().to_seconds(); // actual time
        double wave = 0.0;

        switch(signal_type)
        {
            case 0:
                wave = phase * ampl * std::sin( 2.0 * M_PI * freq * t ) + offset; // sine signal
                break;
            case 1:
                if(std::sin( 2.0 * M_PI * freq * t )>0) //signbit(phase * ampl * std::sin( 2.0 * M_PI * freq * t ) + offset );
                {
                    wave = ( (double) (phase * ampl  + offset ));
                }
                else
                {
                    wave = ( (double) (-1.0 * phase * ampl  + offset ));
                }
                break;
            case 2:
                wave = ampl * phase *(1 - 4*(abs(round(freq*t - 0.25)- (freq*t - 0.25)))) + offset  ;   //value = 1f-4f*(float)Math.Abs( Math.Round(t-0.25f)-(t-0.25f) );
                break;
            case 3:
                wave = 2 * ((freq * t)-round(freq*t +0.5)); // 2f*(t-(float)Math.Floor(t+0.5f));
                break;
            default:
                wave = phase * ampl * std::sin( 2.0 * M_PI * freq * t ) + offset; // sine signal
                break;
        }

        out.write(wave);
    }