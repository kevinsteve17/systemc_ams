/*******************************************************
 *  Instituto Tecnologico de Costa Rica
 *
 *  Diseño de Alto Nivel de Sistema Electrónicos
 *
 *  Proyecto 2 (Grupo 1)
 *
 *  Mario Castro    200827325
 *  Diego Herrera   200324558
 *  Fernando París  200510153
 *  Esteban Rivera  2018319491
 *  Kevin Víquez    200944341
 *******************************************************/

#include "CpuTlmMem/cpu.h"
#include "CpuTlmMem/common_rand.h"
#include "CpuTlmMem/constants.h"

CPU::CPU(sc_core::sc_module_name module_name, unsigned int id)
    : sc_module(module_name),
      init_socket("init_socket"),
      m_payload_event_queue(this, &CPU::peq_cb),
      cpu_id(id),
      request_in_progress(NULL)
{
    init_socket.register_nb_transport_bw(this, &CPU::nb_transport_bw);

    SC_THREAD(thread_process);
}

void CPU::generateTransaction(tlm::tlm_generic_payload* trans, int _data, int addr)
{

    tlm::tlm_phase phase;
    sc_time delay;

        int adr = addr;

        adr = compose_address(0, cpu_id, TOP_ROUTER, adr);

        tlm::tlm_command cmd = tlm::TLM_WRITE_COMMAND; 
        
        if (cmd == tlm::TLM_WRITE_COMMAND) {
            data = _data; 
        }

        // Grab a new transaction from the memory manager
        trans = m_mm.allocate();
        trans->acquire();

        // Set all attributes except byte_enable_length and extensions (unused)
        trans->set_command( cmd );
        trans->set_address( adr );
        trans->set_data_ptr( reinterpret_cast<unsigned char *>(&data) );
        trans->set_data_length( 4 );
        trans->set_streaming_width( 4 ); // = data_length to indicate no streaming
        trans->set_byte_enable_ptr( 0 ); // 0 indicates unused
        trans->set_dmi_allowed( false ); // Mandatory initial value
        trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value

        // Initiator must honor BEGIN_REQ/END_REQ exclusion rule
        if (request_in_progress) {
            wait(end_request_event);
        }

        request_in_progress = trans;
        phase = tlm::BEGIN_REQ;

        // Timing annotation models processing time of initiator prior to call
        delay = sc_time(rand_ps(), SC_PS);
        
        std::cout << ">>>>>>>>>> Outgoing msg from CPU: " << cpu_id
                  << ", Transaction: " << 0
                  << ", Phase: " << phase
                  << ", Cmd: " << (cmd ? 'W' : 'R')
                  << ", Addr: " << dec << decode_addr(adr)
                  << ", Data: " << data
                  << ", SRC: " << decode_src(adr)
                  << ", DST: " << decode_dest(adr)
                  << ", Time: " << sc_time_stamp() << "\n";

        // Non-blocking transport call on the forward path
        tlm::tlm_sync_enum status;
        status = init_socket->nb_transport_fw( *trans, phase, delay );

        // Check value returned from nb_transport_fw
        if (status == tlm::TLM_UPDATED) {

            // The timing annotation must be honored
            m_payload_event_queue.notify( *trans, phase, delay );
        }
        else if (status == tlm::TLM_COMPLETED) {

            // The completion of the transaction necessarily ends the BEGIN_REQ phase
            request_in_progress = 0;

            // The target has terminated the transaction
            check_transaction( *trans );
        }

    wait(sc_time(rand_ps(), SC_NS));

    std::cout << "Done!\n";  
    std::cout << "\n";  
}

int CPU::generateWaveGenInstruction(int amp, int offset, int sel, int freq1, int freq2, int phase)
{
    int instruction = 0;

    instruction = (amp<<3) | (offset<<6) | (freq1<<9) | (freq2<<13) | (phase<<16) | sel;

    cout << "#### INSTRUCTION= " << instruction << " -> ( offset: " << offset << ", amp: " << amp << ", freq1: " << freq1 << ", freq2: " << freq2 << ", waveType: " << sel << ")";
    cout << "\n";
    return instruction;
}

void CPU::thread_process()
{
    tlm::tlm_generic_payload* trans;
    int instruction = 0;

    // amp, offset, sel, freq1, freq2, phase
    /*wait(sc_time(1, SC_MS));
    instruction = generateWaveGenInstruction(5,0,0,1,4,0);
    generateTransaction(trans, instruction);*/  // --> 0 + 4*sin(2k)

    // amp, offset, sel, freq1, freq2, phase
    wait(sc_time(1, SC_MS));
    instruction = generateWaveGenInstruction(4,0,wave_type::sine,2,3,0);
    generateTransaction(trans, instruction, MMIO_WAVE_GEN_ADDR);  // --> 0 + 4*sin(2k)

    // amp, offset, sel, freq1, freq2, phase
    wait(sc_time(2, SC_MS));
    instruction = generateWaveGenInstruction(4,2,wave_type::sine,1,3,0);
    generateTransaction(trans, instruction, MMIO_WAVE_GEN_ADDR);  // --> 3 + 4*sin(1k)  

    // amp, offset, sel, freq1, freq2, phase
    wait(sc_time(2, SC_MS));
    instruction = generateWaveGenInstruction(2,0,wave_type::triangular,10,3,0);
    generateTransaction(trans, instruction, MMIO_WAVE_GEN_ADDR);  // --> 0 + 2*triang(10k)

    // amp, offset, sel, freq1, freq2, phase
    wait(sc_time(2, SC_MS));
    instruction = generateWaveGenInstruction(2,0,wave_type::square,5,3,0);
    generateTransaction(trans, instruction, MMIO_WAVE_GEN_ADDR);  // --> 0 + 2*square(5k)         
}

tlm::tlm_sync_enum CPU::nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& delay)
{
    std::cout << "<<<<<<<<<< Incoming msg received in CPU: " << cpu_id
              << ", Transaction: " << decode_transID(trans.get_address())
              << ", Phase: " << phase
              << ", Cmd: " << (trans.get_command() ? 'W' : 'R')
              << ", Addr: " << dec << decode_addr(trans.get_address())
              << ", Data: " << *reinterpret_cast<int*>(trans.get_data_ptr())
              << ", Time: " << sc_time_stamp() << "\n";

    m_payload_event_queue.notify(trans, phase, delay);

    return tlm::TLM_ACCEPTED;
}

void CPU::check_transaction(tlm::tlm_generic_payload& trans)
{
    if (trans.is_response_error()) {

        char txt[100];
        sprintf(txt, "Transaction returned with error, response status = %s",
        trans.get_response_string().c_str());
        SC_REPORT_ERROR("TLM-2", txt);
    }

    // Allow the memory manager to free the transaction object
    trans.release();
}

void CPU::peq_cb(tlm::tlm_generic_payload& trans, const tlm::tlm_phase& phase)
{
    if (phase == tlm::END_REQ || (&trans == request_in_progress && phase == tlm::BEGIN_RESP))
    {
        std::cout << "->->->->-> Transaction DONE! CPU: " << cpu_id
                  << ", Transaction: " << decode_transID(trans.get_address())
                  << ", Phase: " << phase
                  << ", Cmd: " << (trans.get_command() ? 'W' : 'R')
                  << ", Addr: " << dec << decode_addr(trans.get_address())
                  << ", Time: " << sc_time_stamp() << "\n";

        // The end of the BEGIN_REQ phase
        request_in_progress = NULL;
        end_request_event.notify();
    }

    else if (phase == tlm::BEGIN_REQ || phase == tlm::END_RESP) {

        SC_REPORT_FATAL("TLM-2", "Illegal transaction phase received by initiator");
    }

    if (phase == tlm::BEGIN_RESP) {

        check_transaction( trans );

        trans.set_address(compose_address(decode_transID(trans.get_address()), cpu_id, TOP_ROUTER, decode_addr(trans.get_address())));

        // Send final phase transition to target
        tlm::tlm_phase fw_phase = tlm::END_RESP;
        sc_time delay = sc_time(rand_ps(), SC_PS);

        std::cout << "->->->->-> Outgoing msg from CPU: " << cpu_id
                  << ", Transaction: " << decode_transID(trans.get_address())
                  << ", Phase: " << fw_phase
                  << ", Cmd: " << (trans.get_command() ? 'W' : 'R')
                  << ", Addr: " << dec << decode_addr(trans.get_address())
                  << ", SRC: " << decode_src(trans.get_address())
                  << ", DST: " << decode_dest(trans.get_address())
                  << ", Time: " << sc_time_stamp() << "\n";

        init_socket->nb_transport_fw( trans, fw_phase, delay );
        // Ignore return value
    }
}
