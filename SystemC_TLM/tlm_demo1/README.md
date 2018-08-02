#  TLM DEMO 1: Blocking Transport
The first example demonstrates the basics of TLM blocking transport with the generic payload and simple sockets. The following contents are covered in this example:
- Create SystemC-TLM modules 
- Use of TLM convenience simple initiator/target sockets
- Bind blocking transport interface
- Set and retrieve  attributes from the transaction
- Untimed coding style 


Figure_1  shows the simulation setup of TLM_demo1. It consists  an *initiator* module and a *target* module. The *initiator* module initiates the transactions that randomly READ/WRITE from/to the *target* module through an initiator socket.  And the *target* module prints the received incoming transactions on the screen. The transaction call is passed in between initiators and targets via *tlm_generic_payload*. This example uses the untimed coding style (timeless loosely-timed coding style) that the transaction is sent through the socket using the blocking transport interface.

![](https://vul2cw.dm.files.1drv.com/y4mrxakQ9vvkc3KzfnchRIeV34eFwoGExB-ksmdaQPyUjc-uBQhbc8bDlStPvKabGDFsiGdIXOxjv45NKqnazQQ0mqBy-Ck5kNLepJtFKjEW0aH-UjGbqKtYYdVVvugRN_EcQec7hkL1Q-VTSqikM3-01tHbIHN8g8c3tWLltM9a3xLDs9sRi-NyYyELXyggt43mci_hhGCD2cWI7eIeMqviw?width=602&height=122&cropmode=none) 
*Figure_1. Simulation setup of TLM_Demo1* 

--- 

## 1. Initiator Module
The source code of the *initiator* module can be found in the folder `vp_tourial\tlm_demo\tlm_demo1\initiator.h`



### 1.1 Include Files
The *initiator* module is based on the TLM 2.0 convenience simple initiator socket.  So it needs the appropriate header, in addition to the standard TLM 2.0 header from the utilities directory. Including `<iostream>` in the header to use functions such as `setw()` to format print. 

```C
#include <iomanip>
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"

```


### 1.2 Module Declaration, Interfaces and  Constructor
Simply use SystemC marco `SC_MODULE()` to define module. 

```C
SC_MODULE(initiator){ /* ... */ }
```

The public interface is the TLM 2.0 simple initiator convenience socket, namely *socket*.

```C
//! TLM-2.0 socket, defaults to 32-bits wide, base protocol
tlm_utils::simple_initiator_socket<initiator> socket;
```

The initiator module is responsible for creating a payload for the transportation. 

```C
//! TLM-2.0 generic payload
tlm::tlm_generic_payload  payload;
```

Simply use  SystemC macro `SC_CTOR()` to define the constructor. The colon followed the constructor calls the constructor of the interface  `simple_initiator_socket` and initializes it with name `socket`.   The member function, `thread_main()` is associated with the class as a SystemC thread, using the `SC_THREAD  macro. 

```C
//! Constructor
SC_CTOR(initiator) : socket("socket")
{
  SC_THREAD(thread_main);
}
```

### 1.3 Process Thread 
The *initiators* module has a single SystemC thread `thread_main()` that initiates random read-from / write-to the *target* module. It is called automatically by the SystemC kernel after elaboration (i.e SystemC initialization). From a hardware point of view, the thread  executed the main tasks in the initiator module. 



```C
void thread_main()
{ /* ... */
```

[comment]: <> (Each generic payload transaction has a standard set of bus attributes: command, address, data, byte enables, streaming width, and response status. )

Three attribute in the transaction are used for the basic transaction. The `tlm::tlm_command` indicates the transaction is a type of READ or WRITE.  This command is set randomly. Variable `addr` holds the memory mapped address of the access. It is defined as a fixed width unsigned integer type up to 64 bits `uint64_t`. The data to write or hold readout is defined as type `uint8_t` for a one-byte data access.  Attributes of the generic payload are set using `set_` interfaces. 

```C
tlm::tlm_command cmd   = tlm::TLM_READ_COMMAND; // command
uint64_t  addr = 0x00000000; // address 
uint8_t   data = 0x00 ;      // data 

for (int i = 0; i < 10; i++)
{
  // random generates tlm command of Write or Read
  cmd  = static_cast<tlm::tlm_command>(rand() % 2);
  // random generates access address form 0xAA000000 to 0xAA0000FF
  addr =  0xAA000000 | (rand() % 0xFF);
  
  payload.set_command( cmd );
  payload.set_address( addr );
  payload.set_data_ptr( &data );
  payload.set_data_length( 4 );
  payload.set_streaming_width( 4 ); // = data_length to indicate no streaming
  payload.set_byte_enable_ptr( 0 ); // 0 indicates unused
  payload.set_dmi_allowed( false ); // Mandatory initial value
  payload.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value

  /* ... */
```

Once the payload fields are set up, *b_transport* interface is called to transport the payload to the target and return the result. The transport function requires a time annotation, even when the time is not being used (as in this case). This must be a time variable, not a constant, since the target can update the value. A dummy variable is declared with zero time and passed to the blocking transport function of the socket with the payload.

```C
// Call the transport. Since this is a synchronous model, the target
// should have synchronized, and no additional delay on return.
sc_time delay = SC_ZERO_TIME;
socket->b_transport(payload, delay);
```

The *initiator* simply repeats the above access to the *targert* without any time detail.  At no time does the thread call SystemC `wait()`, so it will not yield execution to the other threads. In the next demo we will add time information in the modeling.  


--- 

## 2. Target Module
The source code of the *target* module can be found in the folder `vp_tourial\tlm_demo\tlm_demo1\target.h`

### 2.1 Include Files
The *target* module is based on the TLM 2.0 convenience simple target socket, so needs the appropriate header from the utilities directory, in addition to the standard TLM 2.0 header. 

> When using the OSCI simulator, it is necessary to define the macro SC_INCLUDE_DYNAMIC_PROCESSES when using certain parts of the TLM-2.0 kit, in particular, the simple sockets, the reason being that these particular sockets spawn dynamic processes.  [doulos.com](https://www.doulos.com/knowhow/systemc/tlm2/tutorial1/)

In this set of demos, the macro `SC_INCLUDE_DYNAMIC_PROCESSES` has been defined in the Cmakefile. Nevertheless, a redefinition won't cause any error.

```C
// Needed for the simple_target_socket
// #define SC_INCLUDE_DYNAMIC_PROCESSES

#include <iomanip>
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h.h"

```

### 2.2 Module Declaration, Interfaces and  Constructor
A constructor is needed, which will be used to register the callback function for the initiator target convenience socket blocking transport. This means that any initiator which requests blocking transport (by calling the initiator socket's b_transport function) will invoke this callback function in the target. 

```C
SC_MODULE(target)
{
  tlm_utils::simple_target_socket<target> socket;

 //! Constructor
 SC_CTOR(target) : socket("socket")
 {
     // Register callback for incoming b_transport interface method call
     socket.register_b_transport(this, &target::b_transport);
 }
 /* ... */
}
```

### 2.3 Blocking Transport
The callback function blocking transport method is implemented in the target module. It extracts and records the information passed in the transaction it receives. 

```C
void b_transport( tlm::tlm_generic_payload& payload, sc_time& delay )
{
  tlm::tlm_command cmd      = payload.get_command();
  sc_dt::uint64    addr     = payload.get_address();
  uint8_t*         data     = payload.get_data_ptr();
       
  //print  log information print
  cout << setw(10) << setfill(' ') << sc_time_stamp();
  cout << " (Target) Logging  "<<endl;
  cout << setw(20) << setfill(' ') <<' ';
  cout << "Command : "<< (cmd ? "write" : "read") <<endl;
  cout << setw(20) << setfill(' ') <<' ';
  cout <<  "Address : 0x" << setw(8) << setfill('0') << hex << addr <<endl;
  if(cmd ==tlm::TLM_WRITE_COMMAND)
  {
      cout << setw(20) << setfill(' ') <<' ';
      cout << "Data    : 0x" << setw(2) << setfill('0');
      cout << hex << (uint32_t)data[0] << endl;
  }

```

The *target* module does neither model time details, it simply ignores the value of the delay argument and returns it to the initiator untouched.
```C
// Successful completion
payload.set_response_status( tlm::TLM_OK_RESPONSE );
/* ... */
```



--- 

## 3. Top level and sc_main Function 
The main function `sc_main` function invokes the simulation. The top-level hierarchy is implemented in the `sc_main` function that instantiates of initiator and target module are connected.   The source code file for the main program can be found in `vp_tourial\tlm_demo\tlm_demo1\sc_main.h` 

```C
int sc_main(int argc, char* argv[])
{
  initiator   *i_initiator   = new initiator("i_initiator");
  target      *i_target      = new target("i_target");
  i_initiator->socket.bind( i_target->socket ); // Bind  ports
  
  sc_start();  // Run forever
  
  return 0;
}
```


--- 


##  4. Simulation and Outputs
Compile and execute TLM_demo1 will print the following outputs:
```
./tlm_demo1

             SystemC 2.3.0-ASI --- May 25 2014 14:05:07
        Copyright (c) 1996-2012 by all Contributors,
        ALL RIGHTS RESERVED

#Test_0
      (Initiator) @ 0 s, Writing 0x0\236 to address  0x08000013
      (Target)    @ 0 s, Logging 
          Command : WRITE
          Address : 0x08000013
          Data    : 0x139E
      (Initiator) @ 0 s, TLM_OK_RESPONSE

#Test_1
      (Initiator) @ 0 s, Reading from address 0x080000AF
      (Target)    @ 0 s, Logging 
          Command : READ
          Address : 0x080000AF
      (Initiator) @ 0 s, TLM_OK_RESPONSE

#Test_2
      (Initiator) @ 0 s, Reading from address 0x08000072
      (Target)    @ 0 s, Logging 
          Command : READ
          Address : 0x08000072
      (Initiator) @ 0 s, TLM_OK_RESPONSE

#Test_3
      (Initiator) @ 0 s, Reading from address 0x080000BC
      (Target)    @ 0 s, Logging 
          Command : READ
          Address : 0x080000BC
      (Initiator) @ 0 s, TLM_OK_RESPONSE

...
```
Each access from the *initiator* generates the expected transactional access. All accesses are 32 bits wide. The *target* always return  *TLM_OK_RESPONSE*. As a timeless model, all events happen with ZERO time notation. 
