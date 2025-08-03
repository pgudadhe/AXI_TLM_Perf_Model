#include "axi_master.h"

void AXIMaster::traffic_generator_method()
{
    // Generate a random transaction
    tlm::tlm_generic_payload trans;
    tlm::tlm_phase phase;
    sc_time delay;
    uint64_t addr = 0;

    // Is it going to be a READ or WRITE command?  0 = READ, 1 = WRITE
    tlm::tlm_command cmd = generate_tlm_command(); // Generate a TLM command based on the configured ratio

    trans.set_command(cmd);
    trans.set_address(0);      // Address is irrelevant for this example
    // trans->set_data_ptr(reinterpret_cast<unsigned char *>(&m_data[i % 16]));

    uint32_t req_size = m_cfg_req_size ? (((m_cfg_req_size + 4) >> 2) << 2) : generate_data_len(); // Use configured size or generate a random
    trans.set_data_length(req_size);
    trans.set_streaming_width(4);                            // = data_length to indicate no streaming
    trans.set_byte_enable_ptr(0);                            // 0 indicates unused
    trans.set_dmi_allowed(false);                            // Mandatory initial value
    trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE); // Mandatory initial value

    // For future when we implement back pressure
    //assert(m_num_trans_in_progress < FIFO_SIZE);
    //m_num_trans_in_progress++;
    
    phase = tlm::BEGIN_REQ;

    // Request delay is 2 clocks
    delay = sc_time(AXI_CLK_PERIOD) * 2;

    //For writes we need to account for the delay
    if (cmd == TLM_WRITE_COMMAND) 
    {
        // How many beats based of the write bus width
        uint32_t beats = trans.get_data_length() / ((m_cfg_axi_write_bus_width + 1) << 2); // Convert bus width to bytes
        delay += sc_time(AXI_CLK_PERIOD) * beats; // Add delay based on the number of beats
        DEBUG_PRINT(3, "Write beats: " << beats << ", delay: " << delay);
    }

    DEBUG_PRINT(2,  "Issueing Req# " << m_transaction_count<< ": " << (cmd ? "WRITE" : "READ") << " request for addr: 0x" << hex << addr << " at time=" << sc_time_stamp() );

    tlm::tlm_sync_enum status = socket->nb_transport_fw(trans, phase, delay);

    if (status == TLM_UPDATED)
    {
        m_stat_rd_delay += delay; // Update statistics for write delay
        peq.notify(trans, phase, delay);
    }
    else if (status == TLM_COMPLETED)
    {
       DEBUG_PRINT(2,  "AXIMaster: Transaction completed at time: " << sc_time_stamp() );
    }
    
    // For future when we implement back pressure
    //m_stat_num_trans++;

    m_transaction_count++;

    if (m_transaction_count < m_cfg_total_transactions)
        m_traffic_generator_event.notify(1, SC_NS);
}

tlm_sync_enum AXIMaster::nb_transport_bw(tlm_generic_payload &trans, tlm_phase &phase, sc_time &delay)
{
    peq.notify(trans, phase, delay);
    return TLM_ACCEPTED;
}

void AXIMaster::peq_cb(tlm_generic_payload &trans, const tlm_phase &phase)
{
    if (phase == BEGIN_RESP)
    {
        tlm_phase fw_phase = END_RESP;
        sc_time delay = SC_ZERO_TIME;
        DEBUG_PRINT(2,  "AXIMaster: READ request completed for transaction at time: " << sc_time_stamp() );
        socket->nb_transport_fw(trans, fw_phase, delay);
    }
}

tlm::tlm_command AXIMaster::generate_tlm_command() 
{
    random_device rd; // Seed for random number generator 
    mt19937 gen(rd()); // Mersenne Twister generator 
    bernoulli_distribution dist(m_cfg_wr_to_rd_ratio); // Bernoulli distribution for Write/Read ratio

    // Generate a command based on the distribution
    // If the distribution returns true, it's a Write command; otherwise, it's a Read command
    if (dist(gen)) 
        return TLM_WRITE_COMMAND; // Write command
    else 
        return TLM_READ_COMMAND; // Read command
}

uint32_t AXIMaster::generate_data_len()
{
    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1024);

    // Generate a random 4-byte aligned length
    int randomLength = dist(gen) * 4;
    return randomLength;
}