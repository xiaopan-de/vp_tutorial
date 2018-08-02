/* *****************************************************************************
 * @file    /vp_tutorial/tlm_demo/tlm_demo2/sc_main.cpp
 * @author  Xiao Pan <pan@cs.uni-kl.de>
 * @date    09.07.2018
 *
 * @brief   Main program of TLM_demo2
 *
 * This file is part of TLM tutorials in the master course "Virtual Prototyping"
 * given by Prof.Dr. Christoph Grimm (TU Kaiserslautern).
 * See: https://cps.cs.uni-kl.de/lehre/virtual-prototyping/
 * ****************************************************************************/

#include <time.h>
#include "memory.h"
#include "processor.h"

using namespace std;

// -----------------------------------------------------------------------------
//! main program to execute TLM_demo2
// -----------------------------------------------------------------------------
int sc_main(int argc, char* argv[])
{
    //! Instantiate the modules
    processor  *i_processor  = new processor("i_processor");
    memory     *i_mem        = new memory("i_memory");
    
    //! Bind  the TLM ports
    i_processor->data_bus.bind( i_mem->data_bus );

    // run forever
    sc_core::sc_start();
    
    
    // print simulation performance
    cout << "\n\n\n";
    cout << "#######################################" << endl;
    cout << "#                                     #" << endl;
    cout << "# TLM_demo 2 : Simulation Complete.   #" << endl;
    cout << "#                                     #" << endl;
    cout << "#######################################" << endl;
    
    return 0;
}



