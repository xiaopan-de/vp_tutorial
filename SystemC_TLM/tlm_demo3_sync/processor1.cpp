/* *****************************************************************************
 * @file    /vp_tutorial/tlm_demo/tlm_demo3_sync/processor1.h
 * @author  Xiao Pan <pan@cs.uni-kl.de>
 * @date    09.07.2018
 *
 * @brief   Processor1 module definition
 *
 * This file is part of TLM tutorials in the master course "Virtual Prototyping"
 * given by Prof.Dr. Christoph Grimm (TU Kaiserslautern).
 * See: https://cps.cs.uni-kl.de/lehre/virtual-prototyping/
 * ****************************************************************************/


#include "processor1.h"


using namespace std;
using namespace sc_core;

//------------------------------------------------------------------------------
//! Class Constructor of the processor0 module
//
//! @param name        SystemC module name
//------------------------------------------------------------------------------
processor1::processor1(sc_core::sc_module_name  name) : processor (name)
{
    
}

// -----------------------------------------------------------------------------
//! The SystemC thread running the TLM access tests of the example.
//
// Demonstrates access byte, half-word, full-word of a 32-bit memory using
// byte_enable_ptr:
// test 1 : read  byte data.
// test 2 : read  half-word data.
// test 3 : read  full-word data.
// test 4 : write byte data.
// test 5 : write half-word data.
// test 6 : write full-word data.
// -----------------------------------------------------------------------------
void processor1::program_main()
{
    uint32_t byte_en = 0x000000FF;    // byte enable ptr
    uint32_t rdata   = 0x00000000;    // data to write to the memory
    uint32_t addr    = 0xFF000000;    // address to write to the memory
    sc_time delay    = SC_ZERO_TIME;  //  time delay
    
    wait(sc_time(25, SC_NS)); // wait until the first data writtten into memory

    
    while(true)
    {
        if(!bus_readwrite(tlm::TLM_READ_COMMAND, addr++, 4,
                          reinterpret_cast<uint8_t *>(&rdata),
                          reinterpret_cast<uint8_t *>(&byte_en))
           ){
            cout << "     (cpu0) @ " << sc_time_stamp() << ", Read Succeeded.\n";
            process_data (rdata);
        }else{
            cout << "     (cpu0) @ " << sc_time_stamp() << ", Read Failed.\n";
        }
    }
}


// -----------------------------------------------------------------------------
// The funtion simulates the instructions execution to process the received data
// before write to the memory by increment of program counter (PC). Assume
// the data prepare program starts from address 0x00000200 in the memory, and
// further assume that it executes 10 instructions to prepare a full- word data.
// -----------------------------------------------------------------------------
void processor1::process_data (uint32_t data)
{
    cout << "(cpu1)      @ " << sc_time_stamp();
    cout << ", data received = 0x";
    cout <<  setw(2) << setfill('0') << hex << uppercase << data << endl;
    
    for (uint32_t pc = 0x00000200; pc<0x00000228; pc+=4)
    {
        // debug print to track the program counter
        cout << "(cpu1)      @ " << sc_time_stamp();
        cout << ", processing received data, PC = 0x";
        cout <<  setw(4) << setfill('0') << hex << uppercase << pc << endl;
        
        wait(sc_time(2, SC_NS)); // wait for the instruction delay
    }
    
    cout << "(cpu1)      @ " << sc_time_stamp();
    cout << ", data processing complete.\n" << endl;
    
    return;
}
