/* *****************************************************************************
 * @file    memory.h
 * @author  Xiao Pan <pan@cs.uni-kl.de>
 * @date    09.07.2018

 * @brief   This file is part of TLM toturials in the master course "Virtual
 *          Prototyping" offered by Prof.Dr. Christoph Grimm (TU Kaiserslautern,
 *          Germany). More information please go to:
 *          https://cps.cs.uni-kl.de/lehre/virtual-prototyping/
 *
 * ****************************************************************************/

#ifndef _tlm_demo2_memory_h_
#define _tlm_demo2_memory_h_

#include <iomanip>
#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;


//------------------------------------------------------------------------------
// Target module representing a simple memory
// 256 Bytes memory size, supports byte, halfword, and fullword access
// with data_ptr structure alignment
//------------------------------------------------------------------------------
struct memory: sc_module
{
    // TLM-2 socket, defaults to 32-bits wide, base protocol
    tlm_utils::simple_target_socket<memory> socket;
    
    // memory size 256 bytes
    enum { MEM_SIZE = 256 };
    
    // memory
    uint8_t mem[MEM_SIZE];
    
    //--------------------------------------------------------------------------
    //! Constructor for memory module
    //
    // - Register the blocking call back func "bus_readwrite" with target socket
    // - Initialize memory with random data
    //--------------------------------------------------------------------------
    SC_CTOR(memory) : socket("socket")
    {
        // Register callback for incoming bus_readwrite interface method call
        socket.register_b_transport(this, &memory::bus_readwrite);
        
        // Initialize memory with random data_ptr
        for (int i = 0; i < MEM_SIZE; i++)  mem[i] = rand() % 0xFF;

        // debug print memory contents
        print_memory();
    }
    
    //--------------------------------------------------------------------------
    //! Blocking transport routine the target socket
    //
    //! @param payload  The generic TLM payload
    //! @param delay    How far the initiator is beyond baseline SystemC time.
    //--------------------------------------------------------------------------
    virtual void bus_readwrite( tlm::tlm_generic_payload& payload, sc_time& delay )
    {
        
        tlm::tlm_command cmd         = payload.get_command();
        sc_dt::uint64    addr        = payload.get_address();
        unsigned char*   data_ptr    = payload.get_data_ptr();
        unsigned int     length      = payload.get_data_length();
        unsigned char*   byte_en_ptr = payload.get_byte_enable_ptr();
        unsigned int     width       = payload.get_streaming_width();
        
        // log information
        cout << setw(10) << setfill(' ') << sc_time_stamp() << ", Memory : ";
        cout << "Command = "<< (cmd ? "write" : "read ");
        cout << "Address = 0x" << setw(8) << setfill('0') << hex
        << (uint64_t)addr  << ", ";
        if(cmd ==tlm::TLM_WRITE_COMMAND)
        {
            cout << "data_ptr = 0x" << setw(2) << setfill('0') << hex
            << *((uint32_t*) data_ptr);
        }
        if(byte_en_ptr != 0)
        {
            cout << "  Byte_Enable = 0x" << setw(8) << setfill('0') << hex
            << *((uint32_t*) byte_en_ptr);
        }
        cout << endl;
    
        // check for unsupported features
        if (length > 4 || width < length)
        {
            payload.set_response_status( tlm::TLM_GENERIC_ERROR_RESPONSE );
            return;
        }
        
        // memory address offset
        uint8_t addr_offset = addr & 0x000000FF;
        
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
                    memcpy(reinterpret_cast<uint8_t*>(&mem_temp), &mem[addr_offset], length);
                    mem_temp &= ~ (*((uint32_t *) byte_en_ptr));
                    mem_temp |=  (*((uint32_t *) data_ptr));
                    memcpy( &mem[addr_offset], reinterpret_cast<uint8_t*>(&mem_temp), length);
                }else
                {
                    memcpy( &mem[addr_offset], reinterpret_cast<uint8_t*>(data_ptr), length);
                }
                print_memory(); // print contents of memory
                break;
            case tlm::TLM_IGNORE_COMMAND:
                break;
            default:
                break;
        }
        
        // Delay as appropriate.
        switch( cmd )
        {
            case tlm::TLM_READ_COMMAND:
                // Represent the delay to access one byte data for read
                wait( sc_time( 5, SC_NS ));
                delay = SC_ZERO_TIME;
                break;
            case tlm::TLM_WRITE_COMMAND:
                // Represent the delay to access one byte data for write
                wait( sc_time( 10, SC_NS ));
                delay = SC_ZERO_TIME;
                break;
            case tlm::TLM_IGNORE_COMMAND:
                delay = SC_ZERO_TIME;
                break;
        }
        
        // successful completion
        payload.set_response_status( tlm::TLM_OK_RESPONSE );
    }
    
    // ---------------------------------------------------------
    // Debug function that prints memory contents for debug use
    // ---------------------------------------------------------
    void print_memory(void)
    {
        cout << setw(20) << setfill(' ') <<' ' << " Memory Updated : " << endl;
        cout << setw(20) << setfill(' ') <<' ' << " +----+----+----+----+ \n";
        cout << setw(20) << setfill(' ') <<' ' << " |... |... |... |... |     ..." << endl;
        for (int i = 16; i > 4 ; )
        {
            cout << setw(20) << setfill(' ') <<' ';
            for (int j = 4; j > 0; j--)
            {
                cout<< " | " << setw(2) << setfill('0') << hex
                << (uint64_t)( mem[--i] );
            }
            cout<< " | --> 0x" << setw(2) << setfill('0') << hex
            << (uint64_t)(i) << endl;
        }
        cout << setw(20) << setfill(' ') <<' ' << " |... |... |... |... |     ..." << endl;
        cout << setw(20) << setfill(' ') <<' ' << " +----+----+----+----+" << endl;
    }

};

#endif

