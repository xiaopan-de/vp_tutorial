/* *****************************************************************************
 * @file    /vp_tutorial/tlm_demo/tlm_demo2/processor.cpp
 * @author  Xiao Pan <pan@cs.uni-kl.de>
 * @date    09.07.2018
 *
 * @brief   Processor module implementation
 *
 * This file is part of TLM tutorials in the master course "Virtual Prototyping"
 * given by Prof.Dr. Christoph Grimm (TU Kaiserslautern).
 * See: https://cps.cs.uni-kl.de/lehre/virtual-prototyping/
 * ****************************************************************************/


#include "processor.h"

SC_HAS_PROCESS( processor );

using namespace std;


//------------------------------------------------------------------------------
//! Class Constructor of the processor module
//
//! @param name        SystemC module name
//------------------------------------------------------------------------------
processor::processor(sc_core::sc_module_name  name):
sc_module (name),
data_bus("data_bus")
{
    //! Defines the function ::program_main() as a SystemC thread.
    SC_THREAD (program_main);
}



// ----------------------------------------------------------------------------
//! Function to handle read and write from the SystemC thread ::program_main().
//
//! @param  cmd           The TLM access command, read or write
//! @param  addr          The address for the access
//! @param  data_len      The number of bytes to read
//! @param  data_ptr      Vector for the access data
//! @param  byte_en_ptr   The byte enable mask for the access
//! @param  delay         The transaction delay
//
//! @return  Zero on success. A return code otherwise.
// ----------------------------------------------------------------------------
int processor::bus_readwrite(tlm::tlm_command  cmd,
                              uint64_t          addr,
                              int               data_len,
                              uint8_t*          data_ptr,
                              uint8_t*          byte_en_ptr,
                              sc_core::sc_time  delay)
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
    
    // Blocking transport call
    data_bus->b_transport(trans, delay);
    
    // For now just simple non-zero return code on error 
    return  trans.is_response_ok () ? 0 : -1;
}



// -------------------------------------------------------------------------
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
// -------------------------------------------------------------------------
void processor::program_main()
{
    // Instruction execution timing
    sc_core::sc_time   ins_delay = sc_core::sc_time(10, sc_core::SC_NS);
    
    uint32_t byte_en = 0xFFFFFFFF;     // for byte enable ptr
    uint32_t data    = 0x00000000;     // for data ptr

    cout << endl;
    cout << "#######################################################" << endl;
    cout << "# Test_1: Read byte data from address 0x08000000.     #" << endl;
    cout << "#######################################################" << endl;
    // set byte enable (mask bytes)
    byte_en = 0x000000FF;
    // delay of the instruction processing time
    wait(ins_delay);
    // bus communication
    if(!bus_readwrite(tlm::TLM_READ_COMMAND, 0x08000000, 4, // cmd,addr,data_len
                  reinterpret_cast<uint8_t *>(&data),       // data_ptr[]
                  reinterpret_cast<uint8_t *>(&byte_en),    // byte_en_ptr[]
                  sc_core::SC_ZERO_TIME)                    // delay
       ){
        cout << "(Processor) @ " << sc_core::sc_time_stamp();
        cout << ", Read succeeded. " << endl;
        cout << "    Readout data : 0x" << setw(2) << setfill('0');
        cout << hex << uppercase << data << endl;
        cout << endl;
    }else{
        cout << "(Processor) @ " << sc_core::sc_time_stamp();
        cout << ", Read failed. " << endl;
    }
    
    cout << endl;
    cout << "#######################################################" << endl;
    cout << "# Test_2: Read half-word data from address 0x08000000.#" << endl;
    cout << "#######################################################" << endl;
    // set byte enable (mask bytes)
    byte_en = 0xFFFF0000;
    // delay of the instruction processing time
    wait(ins_delay);
    // bus communication
    if(!bus_readwrite(tlm::TLM_READ_COMMAND, 0x08000000, 4,  // cmd,addr,data_len
                      reinterpret_cast<uint8_t *>(&data),    // data_ptr[]
                      reinterpret_cast<uint8_t *>(&byte_en), // byte_en_ptr[]
                      sc_core::SC_ZERO_TIME)                 // delay
       ){
        cout << "(Processor) @ " << sc_core::sc_time_stamp();
        cout << ", Read succeeded. " << endl;
        cout << "    Readout data : 0x" << setw(4) << setfill('0');
        cout << hex << uppercase << (data >> 16) << endl; //
        cout << endl;
    }else{
        cout << "(Processor) @ " << sc_core::sc_time_stamp();
        cout << ", Read failed. " << endl;
    }
    
    cout << endl;
    cout << "#######################################################" << endl;
    cout << "# Test_3: Read full-word data from address 0x08000004.#" << endl;
    cout << "#######################################################" << endl;
    // set byte enable (mask bytes)
    byte_en = 0xFFFFFFFF;
    // delay of the instruction processing time
    wait(ins_delay);
    // bus communication
    if(!bus_readwrite(tlm::TLM_READ_COMMAND, 0x08000004, 4,  // cmd,addr,data_len
                      reinterpret_cast<uint8_t *>(&data),    // data_ptr[]
                      reinterpret_cast<uint8_t *>(&byte_en), // byte_en_ptr[]
                      sc_core::SC_ZERO_TIME)                 // delay
       ){
        cout << "(Processor) @ " << sc_core::sc_time_stamp();
        cout << ", Read succeeded. " << endl;
        cout << "    Readout data : 0x" << setw(8) << setfill('0');
        cout << hex << uppercase << data << endl; //
        cout << endl;
    }else{
        cout << "(Processor) @ " << sc_core::sc_time_stamp();
        cout << ", Read failed. " << endl;
    }
    
    
    cout << endl;
    cout << "#######################################################" << endl;
    cout << "# Test_4: Write byte data to address 0x08000008.      #" << endl;
    cout << "#######################################################" << endl;
    // set byte enable (mask bytes)
    byte_en = 0x000000FF;
    // set data to write
    data   = 0x0000000A;
    // delay of the instruction processing time
    wait(ins_delay);
    // bus communication
    if(!bus_readwrite(tlm::TLM_WRITE_COMMAND, 0x08000008, 4,  // cmd,addr,data_len
                      reinterpret_cast<uint8_t *>(&data),    // data_ptr[]
                      reinterpret_cast<uint8_t *>(&byte_en), // byte_en_ptr[]
                      sc_core::SC_ZERO_TIME)                 // delay
       ){
        cout << "(Processor) @ " << sc_core::sc_time_stamp();
        cout << ", Write succeeded. " << endl;
    }else{
        cout << "(Processor) @ " << sc_core::sc_time_stamp();
        cout << ", Write failed. " << endl;
    }
    
    cout << endl;
    cout << "#######################################################" << endl;
    cout << "# Test_5: Write half-word data to address 0x08000000. #" << endl;
    cout << "#######################################################" << endl;
    // set byte enable (mask bytes)
    byte_en = 0x0000FFFF;
    // set data to write
    data   = 0x00001B0B;
    // delay of the instruction processing time
    wait(ins_delay);
    // bus communication
    if(!bus_readwrite(tlm::TLM_WRITE_COMMAND, 0x08000000, 4,  // cmd,addr,data_len
                      reinterpret_cast<uint8_t *>(&data),    // data_ptr[]
                      reinterpret_cast<uint8_t *>(&byte_en), // byte_en_ptr[]
                      sc_core::SC_ZERO_TIME)                 // delay
       ){
        cout << "(Processor) @ " << sc_core::sc_time_stamp();
        cout << ", Write succeeded. " << endl;
    }else{
        cout << "(Processor) @ " << sc_core::sc_time_stamp();
        cout << ", Write failed. " << endl;
    }
    
    cout << "#######################################################" << endl;
    cout << "# Test_6: Write half-word data to address 0x08000004. #" << endl;
    cout << "#######################################################" << endl;

    // set byte enable (mask bytes)
    byte_en = 0xFFFFFFFF;
    // set data to write
    data   = 0x3C2C1C0C;
    // delay of the instruction processing time
    wait(ins_delay);
    // bus communication
    if(!bus_readwrite(tlm::TLM_WRITE_COMMAND, 0x08000004, 4,  // cmd,addr,data_len
                      reinterpret_cast<uint8_t *>(&data),    // data_ptr[]
                      reinterpret_cast<uint8_t *>(&byte_en), // byte_en_ptr[]
                      sc_core::SC_ZERO_TIME)                 // delay
       ){
        cout << "(Processor) @ " << sc_core::sc_time_stamp();
        cout << ", Write succeeded. " << endl;
    }else{
        cout << "(Processor) @ " << sc_core::sc_time_stamp();
        cout << ", Write failed. " << endl;
    }
    
    
}

