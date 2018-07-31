/* *****************************************************************************
 * @file    sc_main.cpp
 * @author  Xiao Pan <pan@cs.uni-kl.de>
 * @date    09.07.2018
 *
 * @brief   This file is part of TLM toturials in the master course "Virtual
 *          Prototyping" offered by Prof.Dr. Christoph Grimm (TU Kaiserslautern,
 *          Germany). More information please go to:
 *          https://cps.cs.uni-kl.de/lehre/virtual-prototyping/
 *
 * ****************************************************************************/


#include "target.h"
#include "initiator.h"


// -----------------------------------------------------------------------------
//! main program of TLM_demo1
// -----------------------------------------------------------------------------
int sc_main(int argc, char* argv[])
{
    // Instantiate the modules
    initiator   *i_initiator   = new initiator("i_initiator");
    target      *i_target      = new target("i_target");
    
    // Bind the TLM ports
    i_initiator->socket.bind( i_target->socket );

    // Run forever
    sc_core::sc_start();
    
    return 0;
}



