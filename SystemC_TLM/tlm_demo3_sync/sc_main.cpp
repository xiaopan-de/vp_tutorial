/* *****************************************************************************
 * @file    /vp_tutorial/tlm_demo/tlm_demo3_sync/sc_main.cpp
 * @author  Xiao Pan <pan@cs.uni-kl.de>
 * @date    09.07.2018
 *
 * @brief   Main program of TLM_demo3 (synchronized version)
 *
 * This file is part of TLM tutorials in the master course "Virtual Prototyping"
 * given by Prof.Dr. Christoph Grimm (TU Kaiserslautern).
 * See: https://cps.cs.uni-kl.de/lehre/virtual-prototyping/
 * ****************************************************************************/


#include <time.h>
#include "../tlm_demo2/memory.h"
#include "processor0.h"
#include "processor1.h"
#include "bus.h"

using namespace std;

// -----------------------------------------------------------------------------
//! main program to execute TLM_demo2
// -----------------------------------------------------------------------------
int sc_main(int argc, char* argv[])
{
    //! Instantiate the modules
    processor0   *i_cpu0 = new processor0("i_cpu0");
    processor1   *i_cpu1 = new processor1("i_cpu1");
    memory       *i_mem  = new memory("i_memory");
    bus          *i_bus  = new bus("i_bus");
    
    //! Bind  the TLM ports
    i_cpu0->data_bus.bind( i_bus->data_bus0 );
    i_cpu1->data_bus.bind( i_bus->data_bus1 );
    i_bus->initiator_socket.bind(i_mem->data_bus);
    
    double  t_sim = 100;  // simulation time in nano second
    
    int t_start=clock();
    sc_start(t_sim, sc_core::SC_NS);
    int t_stop=clock();
    
    double t_cpu  = (t_stop-t_start)/double(CLOCKS_PER_SEC);
    
    
    // print simulation performance
    cout << "\n\n\n";
    cout << "#############################################" << endl;
    cout << "#                                           #" << endl;
    cout << "# TLM_demo 3(decop) : Simulation Complete.  #" << endl;
    cout << "#                                           #" << endl;
    cout << "# Simulated time   : " << setw(10) << setfill(' ') << t_sim     <<" ns          #"<<endl;
    cout << "# Elapsed CPU time : " << setw(10) << setfill(' ') << t_cpu*1e9 <<" ns          #"<< endl;
    cout << "#                                           #" << endl;
    cout << "#############################################" << endl;
    return 0;
}



