/* *****************************************************************************
 * @file    mcu.h
 * @author  Xiao Pan <pan@cs.uni-kl.de>
 * @date    09.07.2018
 *
 * @brief   This file is part of TLM toturials in the master course "Virtual
 *          Prototyping" offered by Prof.Dr. Christoph Grimm (TU Kaiserslautern,
 *          Germany). More information please go to:
 *          https://cps.cs.uni-kl.de/lehre/virtual-prototyping/
 *
 * ****************************************************************************/

#ifndef _tlm_demo2_mcu_h_
#define _tlm_demo2_mcu_h_

#include <iomanip>
#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

//------------------------------------------------------------------------------
// mcu module generating generic payload transactions
//------------------------------------------------------------------------------
struct mcu: sc_module
{
    // TLM-2 socket, defaults to 32-bits wide, base protocol
    tlm_utils::simple_initiator_socket<mcu> socket;
    
    // tlm_generic_payload
    tlm::tlm_generic_payload  payload;
    
    // Construct
    SC_CTOR(mcu) : socket("socket")
    {
        SC_THREAD(process_thread);
    }
    
    
    //-------------------------------------------
    // bus_readwrite()
    //-------------------------------------------
    void bus_readwrite(tlm::tlm_command     &cmd,
                       uint64_t   &addr,
                       uint8_t    data_ptr[],
                       int        data_len,
                       uint8_t    byte_en_ptr[],
                       sc_time    &trans_delay)
    {
        // Obliged to implement read and write commands
        if (cmd == tlm:: TLM_WRITE_COMMAND)
        {

        }
        else if (cmd == tlm::TLM_READ_COMMAND)
        {
        }
        
        // Initialize 8 out of the 10 attributes,
        // byte_enable_length and extensions being unused
        payload.set_command(cmd);
        payload.set_address(addr);
        payload.set_data_ptr(reinterpret_cast<uint8_t *>(&data_ptr));
        payload.set_data_length(data_len);
        payload.set_streaming_width(data_len);//=data_length to indicate no streaming
        payload.set_byte_enable_ptr(reinterpret_cast<uint8_t *>(byte_en_ptr)); // 0 indicates unused
        payload.set_dmi_allowed(false); // Mandatory initial value
        payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
        // Mandatory initial value
        
        

        socket->b_transport(payload, trans_delay);  // Blocking transport call
        

        
        // Initiator obliged to check response status and delay
        if(!payload.is_response_error())
        {
            data_byte =  data;  // readout
            cout << setw(10) << setfill(' ') << sc_time_stamp();
            cout << ", MCU    : Read succeeded. Readout Data = 0x";
            cout << setw(2) << setfill('0')<< hex << (uint64_t)data_byte <<endl;
        }else{
            cout << setw(10) << setfill(' ') << sc_time_stamp();
            cout << ", MCU    : Read failed. " << endl;
        }
    }
    
    // -------------------------------------------------------------------------
    // Test-bench "software" runing in the MCU
    //
    // Demonstrates access byte, half-word, full-word of a 32-bit memory using
    // byte_enable_ptr from 6 tests:
    // test 1 : read  byte data from memory.
    // test 2 : read  half-word data from the memory address in test 3.
    // test 3 : read  full-word data the memory address in test 5.
    // test 4 : write byte data to the memory address in test 1.
    // test 5 : write half-word data to memory.
    // test 6 : write full-word data to  memory.
    // -------------------------------------------------------------------------
    void process_thread()
    {
        // data structure alignment
        struct test_data
        {
            uint8_t  byte;
            uint16_t halfword;
            uint32_t fullword;
        };
        
        // Instruction execution timing
        sc_time    ins_delay = sc_time(5, SC_NS);
        // Repl. transmission time for the backward path
        sc_core::sc_time  tx_delay = sc_time(5, SC_NS);
        sc_core::sc_time  rx_delay = sc_time(5, SC_NS);

        
        // A dummy time delay of zero for blocking transport call
        sc_core::sc_time dummy_delay = sc_core::SC_ZERO_TIME;
        
        uint32_t byte_en   = 0x00000000;     // for byte enable ptr
        uint32_t data      = 0x00000000;     // for data ptr
        unsigned char  data_byte      = 0;   // one  byte data used in test1,4
        unsigned short data_halfword  = 0;   // half word data used in test2,5
        unsigned int   data_fullword  = 0;   // half word data used in test3,6
        
        
       
        //----------------------------------------------------------------+
        // Test 1: read  one byte data (unsigned char) from  memory.      |
        //----------------------------------------------------------------+
        cout << endl << "# Test_1 Reading byte data from address 0xFF000004." << endl;
        
        // enable mask for read one byte data with little-endian format
        byte_en = 0x000000FF;
        
        payload.set_command(tlm::TLM_READ_COMMAND);
        payload.set_address(0xFF000004);
        payload.set_data_ptr(reinterpret_cast<uint8_t*>(&data));
        payload.set_data_length(4);
        payload.set_streaming_width(4);
        payload.set_byte_enable_ptr(reinterpret_cast<uint8_t*>(&byte_en));
        payload.set_dmi_allowed(false); // mandatory initial value
        payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
        
        wait(tx_delay); // delay time of transmitting command
        socket->b_transport(payload, dummy_delay); // b_transport call
        wait(rx_delay); // delay time of receiving data
        
        
        
        
        
        //----------------------------------------------------------------+
        // Test 2: read  halfword data (unsigned short) from  memory.      |
        //----------------------------------------------------------------+
        wait(ins_delay);   // delay for the instruction execution timing
        
        // log information print
        cout << "\n# Test_2 \n"
        << setw(10) << setfill(' ') << sc_time_stamp() <<
        ", MCU    : Reading byte data from address 0xFF000008. " << endl;
        
        // enable mask for read one byte data with little-endian format
        byte_en = 0x0000FFFF;
        
        payload.set_command(tlm::TLM_READ_COMMAND);
        payload.set_address(0xFF000008);
        payload.set_data_ptr(reinterpret_cast<uint8_t*>(&data));
        payload.set_data_length(4);
        payload.set_streaming_width(4);
        payload.set_byte_enable_ptr(reinterpret_cast<uint8_t*>(&byte_en));
        payload.set_dmi_allowed(false); // mandatory initial value
        payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
        
        
        wait(tx_delay); // delay for the data transmission time

        socket->b_transport(payload, dummy_delay); // b_transport call
        
       
        
        
        //----------------------------------------------------------------+
        // Test 3: read  fullword data (unsigned int) from  memory.       |
        //----------------------------------------------------------------+
        wait(ins_delay);   // delay for the instruction execution timing
        
        // log information print
        cout << "\n# Test_3 \n"
        << setw(10) << setfill(' ') << sc_time_stamp() <<
        ", MCU    : Reading byte data from address 0xFF000008. " << endl;
        
        byte_en = 0xFFFFFFFF;
        
        payload.set_command(tlm::TLM_READ_COMMAND);
        payload.set_address(0xFF00000C);
        payload.set_data_ptr(reinterpret_cast<uint8_t*>(&data));
        payload.set_data_length(4);
        payload.set_streaming_width(4);
        payload.set_byte_enable_ptr(reinterpret_cast<uint8_t*>(&byte_en));
        payload.set_dmi_allowed(false); // mandatory initial value
        payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
        
        
        wait(tx_delay); // delay for the data transmission time
        
        socket->b_transport(payload, dummy_delay); // b_transport call
        
        if(!payload.is_response_error())
        {
            data_fullword = data;
            
            cout << setw(10) << setfill(' ') << sc_time_stamp()
            << ", MCU    : Read succeeded. Readout Data = 0x"
            << setw(4) << setfill('0')<< hex << (uint64_t)data_fullword << endl;
        }else{
            cout << setw(10) << setfill(' ') << sc_time_stamp()
            << ", MCU    : Read failed. " << endl;
        }
        
        
        
        //----------------------------------------------------------------+
        //  Test 4: write one byte data  to  memory.                      |
        //----------------------------------------------------------------+
        wait(ins_delay);   // delay for the instruction execution timing
        
        // log information print
        cout << "\n# Test_4 \n" << setw(10) << setfill(' ') << sc_time_stamp()
        << ", MCU    : Writing byte 0xa1 to address 0xF0000004. " << endl;
        
        byte_en = 0x000000FF;
        data    = 0x4a;
        
        payload.set_command(tlm::TLM_WRITE_COMMAND);
        payload.set_address(0xFF000004);
        payload.set_data_ptr(reinterpret_cast<uint8_t*>(&data));
        payload.set_data_length(4);
        payload.set_streaming_width(4);
        payload.set_byte_enable_ptr(reinterpret_cast<uint8_t*>(&byte_en));
        payload.set_dmi_allowed(false); // mandatory initial value
        payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
        
        
        wait(tx_delay); // delay for the data transmission time
        socket->b_transport(payload, dummy_delay); // b_transport call
        
        if(!payload.is_response_error())
        {
            cout << setw(10) << setfill(' ') << sc_time_stamp()
            << ", MCU    : Write succeeded. " << endl;
        }else{
            cout << setw(10) << setfill(' ') << sc_time_stamp()
            << ", MCU    : Read failed. " << endl;
        }
        
        
        //----------------------------------------------------------------+
        //  Test 5: write halfword data  to  memory.                      |
        //----------------------------------------------------------------+
        wait(ins_delay);   // delay for the instruction execution timing
        
        // log information print
        cout << "\n# Test_4 \n" << setw(10) << setfill(' ') << sc_time_stamp();
        cout << ", MCU    : Writing byte 0xa1 to address 0xF0000004. " << endl;
        
        byte_en = 0x0000FFFF;
        data    = 0x5b5c;
        
        payload.set_command(tlm::TLM_WRITE_COMMAND);
        payload.set_address(0xFF000008);
        payload.set_data_ptr(reinterpret_cast<uint8_t*>(&data));
        payload.set_data_length(4);
        payload.set_streaming_width(4);
        payload.set_byte_enable_ptr(reinterpret_cast<uint8_t*>(&byte_en));
        payload.set_dmi_allowed(false); // mandatory initial value
        payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
        
        
        wait(tx_delay); // delay for the data transmission time
        socket->b_transport(payload, dummy_delay); // b_transport call
        
        if(!payload.is_response_error())
        {
            cout << setw(10) << setfill(' ') << sc_time_stamp();
            cout << ", MCU    : Write succeeded. " << endl;
        }else{
            cout << setw(10) << setfill(' ') << sc_time_stamp();
            cout << ", MCU    : Read failed. " << endl;
        }
        
        //----------------------------------------------------------------+
        //  Test 5: write fullword data  to  memory.                      |
        //----------------------------------------------------------------+
        wait(ins_delay);   // delay for the instruction execution timing
        
        // log information print
        cout << "\n# Test_4 \n" << setw(10) << setfill(' ') << sc_time_stamp()
        << ", MCU    : Writing byte 0xa1 to address 0xF0000004. " << endl;
        
        byte_en = 0xFFFFFFFF;
        data    = 0x6d6e6f60;
        
        payload.set_command(tlm::TLM_WRITE_COMMAND);
        payload.set_address(0xFF00000C);
        payload.set_data_ptr(reinterpret_cast<uint8_t*>(&data));
        payload.set_data_length(4);
        payload.set_streaming_width(4);
        payload.set_byte_enable_ptr(reinterpret_cast<uint8_t*>(&byte_en));
        payload.set_dmi_allowed(false); // mandatory initial value
        payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
        
        
        wait(tx_delay); // delay for the data transmission time
        socket->b_transport(payload, dummy_delay); // b_transport call
        
        if(!payload.is_response_error())
        {
            cout << setw(10) << setfill(' ') << sc_time_stamp()
            << ", MCU    : Write succeeded. " << endl;
        }else{
            cout << setw(10) << setfill(' ') << sc_time_stamp()
            << ", MCU    : Read failed. " << endl;
        }
    }
};

#endif
