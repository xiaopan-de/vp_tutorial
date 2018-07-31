/* *****************************************************************************
 * @file    /vp_tutorial/tlm_demo/tlm_demo1/initiator.h
 * @author  Xiao Pan <pan@cs.uni-kl.de>
 * @date    09.07.2018
 *
 * @brief   Initiator module
 *
 * This file is part of TLM tutorials in the master course "Virtual Prototyping"
 * given by Prof.Dr. Christoph Grimm (TU Kaiserslautern).
 * See: https://cps.cs.uni-kl.de/lehre/virtual-prototyping/
 * ****************************************************************************/



#ifndef _tlm_demo1_initiator_h_
#define _tlm_demo1_initiator_h_

#include <iomanip>
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"


using namespace std;

//------------------------------------------------------------------------------
//! Initiator module generats generic payload transactions.
//------------------------------------------------------------------------------
SC_MODULE(initiator)
{
    //! TLM-2.0 socket, defaults to 32-bits wide, base protocol
    tlm_utils::simple_initiator_socket<initiator> socket;
    
    //! TLM-2.0 generic payload
    tlm::tlm_generic_payload  payload;
    
    //--------------------------------------------------------------------------
    //! constructor
    //--------------------------------------------------------------------------
    SC_CTOR(initiator)
    : socket("socket")
    {
        SC_THREAD(program_main);
    }
    
    // -------------------------------------------------------------------------
    //! SystemC thread that initiates radom read/write from/to the target
    // -------------------------------------------------------------------------
    void program_main()
    {
        tlm::tlm_command cmd   = tlm::TLM_READ_COMMAND;
        uint64_t  addr = 0x00000000;
        uint8_t   data = 0x00 ;

        for (int i = 0; i < 10; i++)
        {
            // random generates tlm command of Write or Read
            cmd  = static_cast<tlm::tlm_command>(rand() % 2);
            
            // random generates access address form 0x08000000 to 0x080000FF
            addr =  0x08000000 | (rand() % 0xFF);
            
            // random generates write data up to 0xFF
            if (cmd == tlm::TLM_WRITE_COMMAND) { data = (rand() % 0xFF); }
            
            // loging information
            cout << endl << "#Test_" << dec << i <<endl;
            cout << "(Initiator) @ " << sc_core::sc_time_stamp();
            if(cmd == tlm::TLM_WRITE_COMMAND)
            {
                cout <<", Writing 0x" << setw(2) << setfill('0');
                cout << hex << uppercase << data << " to address  0x";
            }else{
                cout <<", Reading from address 0x";
            }
            cout << setw(8) << setfill('0') << hex << uppercase << addr << endl;
            // end of loging information
            
            // Initialize 8 out of the 10 payload attributes
            // byte_enable_length and extensions being unused
            payload.set_command( cmd );
            payload.set_address( addr );
            payload.set_data_ptr( &data );
            payload.set_data_length( 4 );
            payload.set_streaming_width( 4 ); // = data_length to indicate no streaming
            payload.set_byte_enable_ptr( 0 );    // 0 indicates unused
            payload.set_dmi_allowed( false ); // Mandatory initial value
            payload.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value
        
            // Call the transport. Since this is a synchronous model, the target
            // should have synchronized, and no additional delay on return.
            sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
            socket->b_transport(payload, delay);
        
            // print log response information
            cout << "(Initiator) @ " << sc_core::sc_time_stamp();
            cout << ", " << payload.get_response_string() <<endl ;
        }
    }
};

#endif
