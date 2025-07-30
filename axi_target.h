#ifndef AXI_TARGET_H
#define AXI_TARGET_H   

#include "axi_common.h"

class AXITarget : public sc_module {
public:
    tlm_utils::simple_target_socket<AXITarget> socket;

    //SC_HAS_PROCESS(AXITarget); 

    AXITarget(sc_module_name name) : 
            sc_module(name),
            socket("socket")
    {
        socket.register_nb_transport_fw(this, &AXITarget::nb_transport_fw);
    }   

    tlm_sync_enum nb_transport_fw(tlm_generic_payload& trans, tlm_phase& phase, sc_time& delay) 
    {
        if (phase == BEGIN_REQ) 
        {
            // Address phase
            delay += sc_time(10, SC_NS); // Simulate address phase delay
            wait(delay);

            // Process transaction
            process_transaction(trans);

            phase = BEGIN_RESP;
            return TLM_UPDATED;
        } else if (phase == END_RESP) 
        {
            return TLM_COMPLETED;
        }
        return TLM_ACCEPTED;
    }

    void process_transaction(tlm_generic_payload& trans) 
    {
        // Simulate processing the transaction
        cout << "AXITarget: Processing transaction at time: " << sc_time_stamp() << endl;

        // Set response status
    } 
};

#endif // AXI_TARGET_H
