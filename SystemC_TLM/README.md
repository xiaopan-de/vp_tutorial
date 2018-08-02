# TLM  Demos
This folder contains demo source files for the SytemC-TLM Demos: 

## 1. tlm_demo1 
The first example demonstrates the basics of TLM blocking transport with the generic payload and simple sockets. The following contents are covered in this example:
- Create SystemC-TLM modules 
- Use of TLM convenience simple initiator/target sockets
- Bind blocking transport interface
- Set and retrieve  attributes from the transaction
- Untimed coding style 

## 2. tlm_demo2
The second example demonstrates the loosely-timed coding style through a CPU-Memory usecae. The following contents are covered through this realistic scenario:

- Use of synchronized loosely timed coding style  
- Use of byte enable pointer and byte enable length  attributes 
- Data structure alignment



## 3. tlm_demo3_sync and  tlm_demo3_decop
The thrid exmaple demonstrates temporal decoupling in the loosely-timed coding style to improve simulation speed. The following contents are covered in this example:
- Concepts of temporal decoupling  
- TLM 2.0 Quantum Keepers
- Global time quantum size versus casual errors. 

`tlm_demo3_sync` is a synchronized implementation of TLM_Demo3 that the system consists of two processors (initiator) and one shared memory (target). 
`tlm_demo3_sync` is a temporal decoupled implementation of the same scenario to compare with synchronized version. 
