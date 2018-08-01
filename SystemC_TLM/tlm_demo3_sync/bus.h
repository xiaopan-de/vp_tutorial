/* *****************************************************************************
 * @file    /vp_tutorial/tlm_demo/tlm_demo3/bus.h
 * @author  Xiao Pan <pan@cs.uni-kl.de>
 * @date    09.07.2018
 *
 * @brief   Bus module definition and implementation
 *
 * This file is part of TLM tutorials in the master course "Virtual Prototyping"
 * given by Prof.Dr. Christoph Grimm (TU Kaiserslautern).
 * See: https://cps.cs.uni-kl.de/lehre/virtual-prototyping/
 * ****************************************************************************/

#ifndef _tlm_demo3_bus_h_
#define _tlm_demo3_bus_h_


#include <systemc>
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

// ----------------------------------------------------------------------------
//! System bus module.
//
//! @tparam N_TARGETS   Number of targets on the bus. The router contains
//!                     an array of simple initiator sockets.
// ----------------------------------------------------------------------------
class bus : public sc_core::sc_module
{
public:
    //! @brief The TLM target sockets to receive bus traffic.
    tlm_utils::simple_target_socket<bus>  data_bus0;
    tlm_utils::simple_target_socket<bus>  data_bus1;

    //! @brief The TLM initiator socket to route generic payload transactions
    //! @note   Tagged sockets to be able to distinguish incoming backward path
    //!         calls, because there are multiple initiator sockets.
    tlm_utils::simple_initiator_socket_tagged<bus> initiator_socket;
    
    // -------------------------------------------------------------------------
    //! Custom Constructor for System bus.
    
    //! Register callbacks for incoming interface method calls.
    //! Instantiation of bus initiators and targets.
    
    //! @param name             The SystemC module name
    // -------------------------------------------------------------------------
    SC_CTOR(bus) : data_bus0("data_bus0"), data_bus1("data_bus1")
    {
        // Register callbacks for incoming interface method calls
        data_bus0.register_b_transport(  this, &bus::bus_read_write0);
        data_bus1.register_b_transport(  this, &bus::bus_read_write1);
    }

private:
    
    
    // -------------------------------------------------------------------------
    //！TLM2.0 blocking transport routine for the bus socket
    //
    //! Receives transport requests and route the transaction on the target socket
    //
    //! @param trans  The transaction payload
    //! @param delay  How far the initiator is beyond baseline SystemC time. For
    //!              use with temporal decoupling
    // ----------------------------------------------------------------------------
    void bus_read_write0( tlm::tlm_generic_payload& trans, sc_core::sc_time& delay )
    {
        trans.set_response_status( tlm::TLM_OK_RESPONSE );  // Always OK
        
        initiator_socket->b_transport( trans, delay );
    }
    
    // -------------------------------------------------------------------------
    //！TLM2.0 blocking transport routine for the bus socket
    //
    //! Receives transport requests and route the transaction on the target socket
    //
    //! @param trans  The transaction payload
    //! @param delay  How far the initiator is beyond baseline SystemC time. For
    //!              use with temporal decoupling
    // ----------------------------------------------------------------------------
    void bus_read_write1( tlm::tlm_generic_payload& trans, sc_core::sc_time& delay )
    {
        trans.set_response_status( tlm::TLM_OK_RESPONSE );  // Always OK
        
        initiator_socket->b_transport( trans, delay );
    }
    
};


#endif

