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


#include "processor0.h"


using namespace std;
using namespace sc_core;


//------------------------------------------------------------------------------
//! Class Constructor of the processor0 module
//
//! @param name        SystemC module name
//------------------------------------------------------------------------------
processor0::processor0(sc_module_name  name) : processor (name)
{

}

// -----------------------------------------------------------------------------
//! The SystemC thread running the TLM access tests of the example.
//
// Demonstrates access byte, half-word, full-word of a 32-bit memory using
// byte_enable_ptr:
// -----------------------------------------------------------------------------
void processor0::program_main()
{
    uint32_t byte_en = 0x000000FF;    // byte enable ptr
    uint32_t wdata   = 0x00000000;    // data to write to the memory
    uint32_t addr    = 0xFF000000;    // address to write to the memory
    sc_time delay    = SC_ZERO_TIME;  //  time delay
    
    while(true)
    {
        wdata = prepare_data ();
        
        // bus communication
        if(!bus_readwrite(tlm::TLM_WRITE_COMMAND, addr++, 4,
                          reinterpret_cast<uint8_t *>(&wdata),
                          reinterpret_cast<uint8_t *>(&byte_en))
           ){
            cout << "     (cpu0) @ " << sc_time_stamp() << ", Write Succeeded.\n";
        }else{
            cout << "     (cpu0) @ " << sc_time_stamp() << ", Write Failed.\n";
        }
    }
}



// -----------------------------------------------------------------------------
// The funtion   simulates the instructions execution to process received data
// before write to the memory by increment of program counter (PC). Assume
// the data prepare program starts from address 0x00000100 in the memory, and
// further assume that it executes 10 instructions to prepare a full- word data.
// -----------------------------------------------------------------------------
 uint32_t processor0::prepare_data ()
{
    static uint32_t data = 0;
    
    for (uint32_t pc = 0x00000100; pc<0x00000128; pc+=4)
    {
        // debug print to track the program counter
        cout << "     (cpu0) @ " << sc_time_stamp();
        cout << ", prepareing write data, PC = 0x";
        cout <<  setw(4) << setfill('0') << hex << uppercase << pc << endl;
        
        wait(sc_time(2, SC_NS)); // wait for the instruction delay
    }
    cout << "     (cpu0) @ " << sc_time_stamp();
    cout << ", data prepared, start writing to memory. \n"  << endl;
    
    return data++;
}
