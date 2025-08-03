#include "axi_target.h"

tlm_sync_enum AXITarget::nb_transport_fw(tlm_generic_payload& trans, tlm_phase& phase, sc_time& delay) 
{
    if (phase == BEGIN_REQ) 
    {
        // Process transaction
        tlm_sync_enum retVal = process_transaction(trans, phase, delay);
        return retVal;
    } 
    else if (phase == END_RESP) 
    {
        return TLM_COMPLETED;
    }
    return TLM_ACCEPTED;
}

tlm_sync_enum AXITarget::process_transaction(tlm_generic_payload& trans, tlm_phase& phase, sc_time& delay) 
{
    // Set response status
    trans.set_response_status(TLM_OK_RESPONSE);

    // Is it a READ or WRITE command?
    if(trans.is_write()) 
    {
        //m_wr_req_fifo.write(trans);
       DEBUG_PRINT(2,   "AXITarget: WRITE request received at time: " << sc_time_stamp() );
        m_stat_wr_delay += delay; // Update statistics for write delay
        phase = END_REQ;    
        return TLM_COMPLETED; // For WRITE commands, we complete immediately
    }
    else 
    {
        // How many beats based of the read bus width
        //m_rd_req_fifo.write(trans);
        uint32_t beats = trans.get_data_length() / ((m_cfg_axi_read_bus_width + 1) << 2);
        delay += sc_time(AXI_CLK_PERIOD) * beats; // Add delay based on the number of beats
        phase = BEGIN_RESP; 
        return TLM_UPDATED; // For READ commands, we may need to update later
    }
} 
