/* *****************************************************************************
 * @file    /vp_tutorial/tlm_demo/tlm_demo2/processor.h
 * @author  Xiao Pan <pan@cs.uni-kl.de>
 * @date    09.07.2018
 *
 * @brief   Processor module definition
 *
 * This file is part of TLM tutorials in the master course "Virtual Prototyping"
 * given by Prof.Dr. Christoph Grimm (TU Kaiserslautern).
 * See: https://cps.cs.uni-kl.de/lehre/virtual-prototyping/
 * ****************************************************************************/

#ifndef _tlm_demo2_processor_h_
#define _tlm_demo2_processor_h_

#include <iomanip>
#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"


//------------------------------------------------------------------------------
// SystemC TLM processor module definition.
//------------------------------------------------------------------------------
class processor : public sc_core::sc_module
{
public:

    //! Class Construct
    processor(sc_core::sc_module_name  name);
    
    // TLM-2 socket, defaults to 32-bits wide, base protocol
    tlm_utils::simple_initiator_socket<processor> data_bus;
   
protected:
    
    //! SystemC Thread which will execute the TLM access tests of the example.
    virtual void program_main();
    
    //! The blocking transport routine for the socket.
    virtual int bus_readwrite(tlm::tlm_command     cmd,
                      uint64_t             addr,
                      int                  data_len,
                      uint8_t*             data_ptr,
                      uint8_t*             byte_en_ptr);
    
    //! The generic payload.
    tlm::tlm_generic_payload  trans;

};

#endif
