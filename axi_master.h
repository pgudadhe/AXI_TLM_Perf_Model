#ifndef AXI_MASTER_H
#define AXI_MASTER_H

#include "axi_common.h"

class AXIMaster : public sc_module {
public:
    tlm_utils::simple_initiator_socket<AXIMaster> socket;
    tlm_utils::peq_with_cb_and_phase<AXIMaster> peq;

    //SC_HAS_PROCESS(AXIMaster);  

    AXIMaster(sc_module_name name) : 
        sc_module(name),
        socket("socket"), 
        peq(this, &AXIMaster::peq_cb)
    {
        SC_METHOD(traffic_generator_method);
        sensitive << m_traffic_generator_event;
    }   

    void traffic_generator_method() 
    {
        cout << "Transaction: " << m_transaction_count << " at time: " << clock() << endl;
        m_transaction_count++;

        if (m_transaction_count < m_cfg_total_transactions)
            m_traffic_generator_event.notify(1, SC_NS);

    }

    tlm_sync_enum nb_transport_bw(tlm_generic_payload& trans, tlm_phase& phase, sc_time& delay) 
    {
        peq.notify(trans, phase, delay);
        return TLM_ACCEPTED;
    }

    void peq_cb(tlm_generic_payload& trans, const tlm_phase& phase) {
        if (phase == BEGIN_RESP) {
            tlm_phase fw_phase = END_RESP;
            sc_time delay = SC_ZERO_TIME;
            socket->nb_transport_fw(trans, fw_phase, delay);

            trans.release();
        }
    }

private:
    sc_event m_traffic_generator_event;
    uint32_t m_transaction_count = 0;
    uint32_t m_cfg_total_transactions = 100;
};

#endif // AXI_MASTER_H