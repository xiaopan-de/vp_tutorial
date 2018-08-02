/* *****************************************************************************
 * @file    /vp_tutorial/SystemC_TLM/demo2/memory.h
 * @author  Xiao Pan <pan@cs.uni-kl.de>
 * @date    09.07.2018
 *
 * @brief   Memory module definition
 *
 * This file is part of TLM tutorials in the master course "Virtual Prototyping"
 * given by Prof.Dr. Christoph Grimm (TU Kaiserslautern).
 * See: https://cps.cs.uni-kl.de/lehre/virtual-prototyping/
 * ****************************************************************************/

#ifndef _tlm_demo2_h_
#define _tlm_demo2_h_

// Needed for the simple_target_socket
// #define SC_INCLUDE_DYNAMIC_PROCESSES

#include <iomanip>
#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"



//------------------------------------------------------------------------------
//! SystemC-TLM Memory module definition.
//------------------------------------------------------------------------------
class memory: sc_core::sc_module
{
public:
    
    //! Class constructor.
    memory(sc_core::sc_module_name  name);
    
    //! TLM-2 socket, defaults to 32-bits wide, base protocol.
    tlm_utils::simple_target_socket<memory> data_bus;
    
private:
    
    //! Memory size 256 bytes.
    static const int MEM_SIZE = 256 ;
    
    //! Byte array models memory storages.
    uint8_t mem[MEM_SIZE];


    //! Blocking transport routine the target socket.
    void bus_readwrite(tlm::tlm_generic_payload& payload,
                       sc_core::sc_time& delay);
   
    //! Prints first n bytes data in the memory for the debug purpose.
    void print_memory(int n);
};

#endif
