/* *****************************************************************************
 * @file    /vp_tutorial/tlm_demo/tlm_demo3_decop/processor1.h
 * @author  Xiao Pan <pan@cs.uni-kl.de>
 * @date    09.07.2018
 *
 * @brief   Processor0 module definition
 *
 * This file is part of TLM tutorials in the master course "Virtual Prototyping"
 * given by Prof.Dr. Christoph Grimm (TU Kaiserslautern).
 * See: https://cps.cs.uni-kl.de/lehre/virtual-prototyping/
 * ****************************************************************************/

#ifndef _tlm_demo3_decop_processor1_h_
#define _tlm_demo3_decop_processor1_h_

#include "../tlm_demo2/processor.h"
#include "tlm_utils/tlm_quantumkeeper.h"


//------------------------------------------------------------------------------
// Extended SystemC TLM processor0 from base class processor in TLM demo2
//------------------------------------------------------------------------------
class processor1 : public  processor
{
public:
    
    //! Class Construct
    processor1(sc_core::sc_module_name  name);

    
private:
    
    //! SystemC Thread which will execute the TLM access tests of the example.
    virtual void program_main();
    
    // function to simulate the data processing program
    void process_data (uint32_t data);
    
    //! The blocking transport routine for the socket.
    int bus_readwrite(tlm::tlm_command     cmd,
                      uint64_t             addr,
                      int                  data_len,
                      uint8_t*             data_ptr,
                      uint8_t*             byte_en_ptr);
    
    //global quantum.
    tlm::tlm_global_quantum *g_quantum;
    
    // Quantum keeper for the ISS model thread.
    tlm_utils::tlm_quantumkeeper  q_keeper;
    
};

#endif
