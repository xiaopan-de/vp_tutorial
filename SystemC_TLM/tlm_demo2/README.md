#  TLM DEMO 2: Memory Mapped Access 
In the previous TLM_Demo1, we introduced the TLM2.0 convenience initiator/target sockets, setting and retrieving information in the payload. In tutorial TLM_Demo2, we will introduce advanced operations based on TLM_Demo1.  Figure_1  shows the simulation setup. It consists of two modules, a  *processor*  and a *memory*.  Processor-Memory access is a typical TLM application. In this example, we will demonstrate load/store a memory mapped data. The following contents are covered through this realistic scenario:

- Use of synchronized loosely timed coding style  
- Use of byte enable pointer and byte enable length  attributes 
- Data structure alignment 

![](https://wrsslg.dm.files.1drv.com/y4mYJNcfCCasRlDfjGmuHIVpgHne8UWeffCrLV1WE6jVwyYe8EbqSfWTsYklVoxGAl5CfmHtGOqgZfa6Su7h3sfqkuArqF2niBW1xjz4g9ZQYMDnB6rBBonmQ_ht5PUefUpH0VrylArqO72XfYhW0YRNTu12jd71VSG3IM8YW_5bmxQmDcN0KRliW2-uJQAK-hiD9ETQ6iviQThluWHwG0qyA?width=1003&height=212&cropmode=none) 
*Figure_1. Simulation setup of TLM_Demo2* 



## 1. Data structure alignment 
A **word** is the natural unit of data processed by the processor. The number of bits in one word is named **wordsize**.  Common computer architectures often have 8, 16, 32 or 64 bits.  But the modern memory is usually **byte** addressable. In this example, the _processor_  module has a word-size of 32 bits (use default TLM socket width), and the _memory_ module is byte addressable. 

Assume the following data structure stored in the _memory_ module: 
```C
struct test_s
{
  char   a;  // 1 bytes 
  short  b;  // 2 bytes 
  int    c;  // 4 bytes 
};
```
And we further assume the data stored in the memory with a starting address 0x00000000. Figure_2 shows the memory layout in the case without alignment. 

![](https://pkrg2w.dm.files.1drv.com/y4m9kapisdo4acptt2brCKOjOYRHmuOKbI5D31gGWRLiILcuadX80fN2mKbAn4cJf-y_rCvTq4Px9SzHm1wR8GANtmI6c52GJqK4WMXs9Pc8fcYQ9UH8oukL9PWbA_xnQd664tMcbdFVi-wjt72prnIrQ_AYAA44wd7Shl_eX-nQRNkknV4h50nQETWfQX1MvRYqGNg-0ltjOcSEH7d1s1WOQ?width=1044&height=188&cropmode=none)
*Figure_2. Misaligned memory* 

The problem is clear shown that the integer data 'c' is stored in two separate words. Therefore the processor would need two memory accesses and bit shifts operations, causing a reduced efficiency. To overcome this issue, compilers will automatically add padding byte(s) to force data aligned in the memory. In the C/C++ program, the above data memory is aligned as shown in Figure_3. The struct consists of in total 7 Bytes data but consumes 8 Byte memory size. Use function `sizeof()`  can you check the result. 


![](https://6hbwga.dm.files.1drv.com/y4mM82bToFC0IasJOoJCE_RewHhW2QR20LEd60_1a48JDceTO5QfEuWF8X_Yr-I-cPFAJVCv9KkJxdirb09vtkll7PLE9b0lHbsv1jcHqIKxcMMe9pGPS06qdfR6N213O8mVtkoSBxR7m5vJ68L7udyri7BrazHyedxSW1WhgRmC3hahnFwPNuuQTYi2YMGN5EYNZvA-zu1erR6vDVT1P5YIA?width=1044&height=194&cropmode=none)
*Figure_2.  Aligned memory with padding* 


In this TLM demo, we will use the same data struct alignment as a test case. The _processor_ module initializes the load/store operations to such data stored in the _memory_ module. 


## 2. Timing 
TLM_Demo1 is untimed. In this demo, time  is added to the modules and synchronize explicitly with SystemC kernel. The following time information is considered: 

- **Execution time**  represents the time required to process instructions, e.g. address checking, to load/store memory access. This time delay can be estimated by counting the number of instructions of a program divided by IPS (instructions per second). In this example, we assume the _processor_ unit performs of 1000 MIPS at its clock frequency. We also assume it executes 10 instructions to load/store one word size data. The execution time is then **10ns per write/read**. 

- **Transmission delay** is the time required to transmit some data. This time delay can be estimated from the data length divided by the transmission speed. Assume we have a transmission speed of 1GB/s for both read and write access. Since the processor always access 4bytes (wordsize) data, the transmission delay is defined as 4 byte / 1GB/s = **4ns per word**. 



--- 



## 3. Memory Module 
The source code of the _memory_ module (memory.cpp and memory.h) may be found in the  `vp_tourial\tlm_demo\tlm_demo2\` directory of the distribution.  The definition and implementation are divided into head and .cpp file. 



### 3.1 Memory Module Class Definition   
The source code of the _memory_ module definition may be found in `vp_tourial\tlm_demo\tlm_demo2\memory.h`.


#### 3.1.1  Include Files
The same as the *target* module in [TLM_Demo1](https://). 

####  3.1.2 Module Declaration and Constructor
Module _memory_ is declared using derived calss of `sc_core::sc_module` which is  equivalent to the SystemC marco  `SC_MODULE`.  By using `SC_MODULE` all variale and functions are by default `public`. This tutorial use explicit declarations of classes derived from sc_module rather than the `SC_MODULE` macro. The constructor specifies the module name. And the default destructor is sufficient. 


```C
class memory : sc_core::sc_module
{
public:
  memory(sc_core::sc_module_name  name);
```

####  3.1.3 Public Interface  
The public interface is the simple target convenience socket.
 
```C
tlm_utils::simple_target_socket<memory> data_bus;
```


####  3.1.4 Memory Array 
Static declared `uint8_t` (`char`) array with default size of 256 is used to keep the contents of the _memory_. 
 
```C
private:

  //! Memory size 256 bytes.
  static const int MEM_SIZE = 256 ;
  
  //! Byte array models memory storages.
  uint8_t mem[MEM_SIZE];
```

####  3.1.5 Blocking Transport
Blocking transport is via a callback function. All the behavior of the module is captured in this callback function. There are no SystemC threads required. 

```C
void bus_readwrite(tlm::tlm_generic_payload& payload, sc_core::sc_time& delay );
```


####  3.1.6 Print Function for Debug 
A function `print_memory` prints first `n` word data in the memory for the debug purpose. 

```C
void print_memory(int n);
```


### 3.2 Memory Module Class Implementation 
The code for the _memory_ module implementation may be found in `vp_tourial\tlm_demo\tlm_demo2\memory.cpp`.



#### 3.2.1 Included Headers
Module definition file `memory.h` in the same directory should be included. 

```C
#include "memory.h"
```


#### 3.2.3 Constructor 
The constructor passes its argument (the module) name to the base class sc_module constructor and initializes tlm simple socket `data_bus`. It then registers the `bus_readwrite` function as the callback for blocking transport to this convenience socket `data_bus`.   


```C
memory::memory(sc_core::sc_module_name  name): sc_module (name), data_bus("data_bus")
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
      
  //! Load test data into memory module from the stating address 0x00.
  memcpy(&mem[0x00], &test_data, sizeof(test_data));
      
  // Print memory contents for debug purpose
  print_memory(3);
}
```

The _memory array_ is initialized with random numbers and a test struct is defined and copied to the memory array from starting address.  In the end, the debug print function `print_memory()` is called to virtualize the contents of the _memory_ which gives the following output:
```
(Memory) @ 0 s, updated
 +----+----+----+----+ 
 | 50 | 78 | 6D | BC | 0x00000008
 +----+----+----+----+ 
 | C3 | C2 | C1 | C0 | 0x00000004
 +----+----+----+----+ 
 | B1 | B0 | 06 | A0 | 0x00000000
 +----+----+----+----+ 
```

As expected, data `test_data.a = 0xA0` is stored in the 1st byte of the word at the address 0x0000000, but it consumes two bytes(half word). Data `test_data.b = 0xB1B0` is stored in the 3rd and 4th bytes of the word at the address 0x0000000.  Data `test_data.c = 0xC3C2C1C0`  occupies the whole word at address 0x0000004. 


#### 3.2.4 Blocking Transport Callback
The blocking transport callback function processes the transaction it received.  First the information from the  generic payload is extracted to the local variables and printed on the screen. 


```C
void memory::bus_readwrite( tlm::tlm_generic_payload& payload,
                           sc_core::sc_time& delay )
{
    
    tlm::tlm_command cmd         = payload.get_command();
    sc_dt::uint64    addr        = payload.get_address();
    unsigned char*   data_ptr    = payload.get_data_ptr();
    unsigned int     length      = payload.get_data_length();
    unsigned char*   byte_en_ptr = payload.get_byte_enable_ptr();
    unsigned int     width       = payload.get_streaming_width();
    
    // log information (ignored)
```

The behavior of the memory is implemented depeneds to the access it recevied. For a READ access, it copies the memory contents from the starting address `addr_offset` of `length` bytes to the data pointer `data_ptr` in the payload. And mask the expected content bytes using the byte enable pointer `byte_en_ptr`.  As a 32bit-word access, a fixed length of 4 bytes is assumed for the data in the payload.


```C
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
        print_memory(12); // print contents of memory
        break;
    case tlm::TLM_IGNORE_COMMAND:
        break;
    default:
        break;
}
```


Since it is a synchronized model, appropriate delays should added to the delay variable according to the access type. As we presented previously, delay of 5ns for both read and write access are defined. 
```C
// Delay as appropriate.
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

```


--- 

## 4. Processor  Module 
The code for the *processor*  module (processor.cpp and processor.h) may be found in the vp_tourial\tlm_demo\tlm_demo2\ directory of the distribution. 

### 4.1  Processor Module Class Definition

#### 4.1.1   Included Headers 
#### 4.1.2   Module Declaration and Constructor
#### 4.1.3   Public Interface 
#### 4.1.4   Blocking transport routine
#### 4.1.5   Thread



### 4.2  Processor Module Class Implementation
#### 4.2.1   Included Headers 
#### 4.2.2   Constructor
#### 4.2.4   Blocking transport routine
#### 4.2.5   Thread






--- 

## 5. Top level and sc_main Function  


--- 

## 6. Simulation and Outputs

[comment]: <> (include more complex use-case : mcu as initiator and memory as target
byte, half word, full-word access from bus communicaion
memory handles the received data
logging)


 
