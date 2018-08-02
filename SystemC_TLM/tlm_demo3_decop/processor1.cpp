/* *****************************************************************************
 * @file    /vp_tutorial/tlm_demo/tlm_demo3_decop/processor1.h
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
    g_quantum = &(tlm::tlm_global_quantum::instance());
    q_keeper.set_global_quantum( g_quantum->get() );
    q_keeper.reset();    // Zero local time offset
}


// ----------------------------------------------------------------------------
//! Function to handle read and write from the SystemC thread ::program_main().
//
//! @param  cmd           The TLM access command, read or write
//! @param  addr          The address for the access
//! @param  data_len      The number of bytes to read
//! @param  data_ptr      Vector for the access data
//! @param  byte_en_ptr   The byte enable mask for the access
//
//! @return  Zero on success. A return code otherwise.
// ----------------------------------------------------------------------------
int processor1::bus_readwrite(tlm::tlm_command  cmd,
                              uint64_t          addr,
                              int               data_len,
                              uint8_t*          data_ptr,
                              uint8_t*          byte_en_ptr)
{
    // Initialize 8 out of the 10 attributes,
    // byte_enable_length and extensions being unused
    trans.set_command(cmd);
    trans.set_address(addr);
    trans.set_data_ptr(data_ptr);
    trans.set_data_length(data_len);
    trans.set_streaming_width(data_len);//=data_length indicates no streaming
    trans.set_byte_enable_ptr(byte_en_ptr);
    trans.set_dmi_allowed(false);
    trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    
    //  time delay
    sc_core::sc_time  delay = q_keeper.get_local_time();
    
    // Blocking transport call
    data_bus->b_transport(trans, delay);
    
    // use td instead of wait to update local time
    q_keeper.set( delay );
    if( q_keeper.need_sync() ) { q_keeper.sync(); } // Sync if needed
    
    
    // For now just simple non-zero return code on error
    return  trans.is_response_ok () ? 0 : -1;
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
        
        // wait for the instruction delay
        q_keeper.inc(sc_time(2, SC_NS));
        if( q_keeper.need_sync() ) q_keeper.sync(); // Sync if needed
    }
    
    cout << "(cpu1)      @ " << sc_time_stamp();
    cout << ", data processing complete.\n" << endl;
    
    return;
}
