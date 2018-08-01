/* *****************************************************************************
 * @file    /vp_tutorial/tlm_demo/tlm_demo3_sync/processor0.h
 * @author  Xiao Pan <pan@cs.uni-kl.de>
 * @date    09.07.2018
 *
 * @brief   Processor0 module definition
 *
 * This file is part of TLM tutorials in the master course "Virtual Prototyping"
 * given by Prof.Dr. Christoph Grimm (TU Kaiserslautern).
 * See: https://cps.cs.uni-kl.de/lehre/virtual-prototyping/
 * ****************************************************************************/

#ifndef _tlm_demo3_sync_processor0_h_
#define _tlm_demo3_sync_processor0_h_

#include "../tlm_demo2/processor.h"

//------------------------------------------------------------------------------
// Extended SystemC TLM processor0 from base class processor in TLM demo2
//------------------------------------------------------------------------------
class processor0 : public  processor
{
public:
    
    //! Class Construct
    processor0(sc_core::sc_module_name  name);
    
    
private:
    
    //! SystemC Thread which will execute the TLM access tests of the example.
    void program_main();
    
    // function to simulate the data prepareing program
     uint32_t prepare_data();
};

#endif
