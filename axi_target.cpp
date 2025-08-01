#include "axi_target.h"

tlm_sync_enum AXITarget::nb_transport_fw(tlm_generic_payload& trans, tlm_phase& phase, sc_time& delay) 
{
    if (phase == BEGIN_REQ) 
    {
        // Process transaction
        process_transaction(trans);

        phase = BEGIN_RESP;
        cout << "AXITarget: Received transaction at time: " << sc_time_stamp() << endl;
        return TLM_UPDATED;
    } else if (phase == END_RESP) 
    {
        return TLM_COMPLETED;
    }
    return TLM_ACCEPTED;
}

void AXITarget::process_transaction(tlm_generic_payload& trans) 
{
    // Simulate processing the transaction
    cout << "AXITarget: Processing transaction at time: " << sc_time_stamp() << endl;

    // Set response status
    trans.set_response_status(TLM_OK_RESPONSE);
} 
