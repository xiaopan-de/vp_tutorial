/* *****************************************************************************
 * @file    /vp_tutorial/SystemC_TLM/demo2/memory.cpp
 * @author  Xiao Pan <pan@cs.uni-kl.de>
 * @date    09.07.2018
 *
 * @brief   Memory module implementation
 *
 * This file is part of TLM tutorials in the master course "Virtual Prototyping"
 * given by Prof.Dr. Christoph Grimm (TU Kaiserslautern).
 * See: https://cps.cs.uni-kl.de/lehre/virtual-prototyping/
 * ****************************************************************************/

#include "memory.h"

using namespace sc_core;
using namespace std;

//------------------------------------------------------------------------------
//! Class Constructor of the memory module
//
//! Registers the blocking call back function "bus_readwrite" with target
//! data_bus.
//! The memory array is first initialized with random numbers. A test struct
//! data is then loaded  to the memory array from the starting address 0x00.
//------------------------------------------------------------------------------
memory::memory(sc_module_name  name) :
sc_module (name), data_bus("data_bus")
{
    //! Register callback for incoming bus_readwrite interface method call.
    data_bus.register_b_transport(this, &memory::bus_readwrite);
    
    //! Initialize memory with random data.
    for (int i = 0; i < MEM_SIZE; i++)  mem[i] = rand() % 0xFF;
    
    //! Test data struct.
    struct test_s
    {
        char  a = 0xA0;       // 1 byte
        short b = 0xB1B0;     // 2 bytes
        int   c = 0xC3C2C1C0; // 4 bytes

    } test_data ;
    
    // Print the memory size of the test data
    // cout<<"size of test data class is " << dec << sizeof(test_s)<<endl;
    
    //! Load test data into memory module from the stating address 0x00.
    memcpy(&mem[0x00], &test_data, sizeof(test_data));
    
    // Print memory contents for debug purpose
    print_memory(3);
}



//------------------------------------------------------------------------------
//! Blocking transport callback function  processes the transaction it received.
//
//! @param payload  The generic TLM payload
//! @param delay    How far the initiator is beyond baseline SystemC time.
//------------------------------------------------------------------------------
void memory::bus_readwrite( tlm::tlm_generic_payload& payload,
                           sc_time& delay )
{
    
    tlm::tlm_command cmd         = payload.get_command();
    sc_dt::uint64    addr        = payload.get_address();
    unsigned char*   data_ptr    = payload.get_data_ptr();
    unsigned int     length      = payload.get_data_length();
    unsigned char*   byte_en_ptr = payload.get_byte_enable_ptr();
    unsigned int     width       = payload.get_streaming_width();
    
    // logging
    cout << "(Memory)    @ " << sc_time_stamp() << ", Logging "  << endl;
    cout << "    Command : " << (cmd ? "WRITE" : "READ") <<endl;
    cout << "    Address : 0x" << setw(8) << setfill('0') << hex << uppercase;
    cout << addr << endl;
    if( cmd == tlm::TLM_WRITE_COMMAND )
    {
        cout << "    Data    : 0x" << setw(2) << setfill('0');
        cout << hex << uppercase << *((uint32_t*) data_ptr) << endl;
    }
    if( byte_en_ptr != 0 )
    {
        cout << "    Byte_en : 0x" << setw(8) << setfill('0');
        cout << hex << uppercase << *((uint32_t*) byte_en_ptr) << endl;
    }
    cout << endl;
    // end of logging

    
    
    // check for unsupported features
    if (length > 4 || width < length)
    {
        payload.set_response_status( tlm::TLM_GENERIC_ERROR_RESPONSE );
        return;
    }
    
    // memory address offset
    uint8_t addr_offset = addr & 0xFF;
    
    // implement read and write commands
    switch( cmd )
    {
        case tlm::TLM_READ_COMMAND:
            memcpy(data_ptr, &mem[addr_offset], length);
            if(byte_en_ptr != 0)
            {
                *((uint32_t *) data_ptr) &= *((uint32_t *) byte_en_ptr);
            }
            break;
        case tlm::TLM_WRITE_COMMAND:
            if(byte_en_ptr != 0)
            {
                uint32_t mem_temp;
                memcpy(reinterpret_cast<uint8_t*>(&mem_temp),
                       &mem[addr_offset],length);
                mem_temp &= ~ (*((uint32_t *) byte_en_ptr));
                mem_temp |=  (*((uint32_t *) data_ptr));
                memcpy( &mem[addr_offset],
                       reinterpret_cast<uint8_t*>(&mem_temp), length);
            }else{ // byte enable ptr not used.
                memcpy( &mem[addr_offset],
                       reinterpret_cast<uint8_t*>(data_ptr), length);
            }
            print_memory(3); // print contents of memory
            break;
        case tlm::TLM_IGNORE_COMMAND:
            break;
        default:
            break;
    }
    
    // add delay as appropriate
    switch( cmd )
    {
        case tlm::TLM_READ_COMMAND:
            // Represent the delay to access one byte data for read
            delay += sc_time( 5, SC_NS );
            break;
        case tlm::TLM_WRITE_COMMAND:
            // Represent the delay to access one byte data for write
            delay += sc_time( 5, SC_NS );
            break;
        case tlm::TLM_IGNORE_COMMAND:
            delay = SC_ZERO_TIME;
            break;
    }
    
    // successful completion
    payload.set_response_status( tlm::TLM_OK_RESPONSE );
}

// -----------------------------------------------------------------------------
//! Prints memory contents for a given length of words
//
//! @param n  Number of words(4bytes) to print out
// -----------------------------------------------------------------------------
void memory::print_memory(int n)
{
    cout << "(Memory) @ " << sc_time_stamp() <<", updated" << endl;

    int n_byte = n * 4;
    if(n_byte > MEM_SIZE) n_byte = MEM_SIZE;
    
    cout << " +----+----+----+----+ " << endl ;
    for (int i = n_byte-1; i>0;)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << " | " << setw(2) << setfill('0') << hex << uppercase ;
            cout << (uint64_t)( mem[i--] );
        }
        cout << " | 0x" << setw(8) << setfill('0') << hex << uppercase ;
        cout << (uint64_t)(i+1) << endl ;
        cout << " +----+----+----+----+ " << endl ;
    }
    
    cout << endl;
}


