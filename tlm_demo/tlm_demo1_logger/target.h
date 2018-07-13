/* *****************************************************************************
 * @file    target.h
 * @author  Xiao Pan <pan@cs.uni-kl.de>
 * @date    09.07.2018
 *
 * @brief   This file is part of TLM toturials in the master course "Virtual
 *          Prototyping" offered by Prof.Dr. Christoph Grimm (TU Kaiserslautern,
 *          Germany). More information please go to:
 *          https://cps.cs.uni-kl.de/lehre/virtual-prototyping/
 *
 * ****************************************************************************/

#ifndef _tlm_demo1_target_h_
#define _tlm_demo1_target_h_

#include <iomanip>
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;


//------------------------------------------------------------------------------
//! Target module responds to the transcation call.
//------------------------------------------------------------------------------
SC_MODULE(target)
{
    //! TLM-2 socket, defaults to 32-bits wide, base protocol
    tlm_utils::simple_target_socket<target> socket;
    
    //--------------------------------------------------------------------------
    //! constructor
    //--------------------------------------------------------------------------
    SC_CTOR(target)
    : socket("socket")
    {
        // Register callback for incoming b_transport interface method call
        socket.register_b_transport(this, &target::b_transport);
    }
    
    //--------------------------------------------------------------------------
    //! Blocking transport routine the target socket.
    //! Simply loging generic payload informtion from initiator.
    //
    //! @param payload  The generic TLM payload
    //! @param delay    How far the initiator is beyond baseline SystemC time.
    //--------------------------------------------------------------------------
    void b_transport( tlm::tlm_generic_payload& payload, sc_time& delay )
    {
        tlm::tlm_command cmd      = payload.get_command();
        sc_dt::uint64    addr     = payload.get_address();
        uint8_t*         data     = payload.get_data_ptr();
        
        //print  log information print
        cout << setw(10) << setfill(' ') << sc_time_stamp();
        cout << " (Target) Logging  "<<endl;
        cout << setw(20) << setfill(' ') <<' ';
        cout << "Command : "<< (cmd ? "write" : "read") <<endl;
        cout << setw(20) << setfill(' ') <<' ';
        cout <<  "Address : 0x" << setw(8) << setfill('0') << hex << addr <<endl;
        if(cmd ==tlm::TLM_WRITE_COMMAND)
        {
            cout << setw(20) << setfill(' ') <<' ';
            cout << "Data    : 0x" << setw(2) << setfill('0');
            cout << hex << (uint32_t)data[0] << endl;
        }
        
        // Successful completion
        payload.set_response_status( tlm::TLM_OK_RESPONSE );
        

    }
};

#endif
