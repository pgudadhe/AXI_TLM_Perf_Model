#ifndef AXI_TARGET_H
#define AXI_TARGET_H   

#include "axi_common.h"

class AXITarget : public sc_module {
public:
    tlm_utils::simple_target_socket<AXITarget> socket;

    //SC_HAS_PROCESS(AXITarget); 

    AXITarget(sc_module_name name, uint32_t read_bus_width) : 
            sc_module(name),
            m_cfg_axi_read_bus_width(read_bus_width),
            socket("socket")
    {
        socket.register_nb_transport_fw(this, &AXITarget::nb_transport_fw);
    }   

    tlm_sync_enum nb_transport_fw(tlm_generic_payload& trans, tlm_phase& phase, sc_time& delay) ;
    void process_transaction(tlm_generic_payload& trans) ;

private:
    uint32_t m_cfg_axi_read_bus_width = 0; // 0:4B, 1:8B, 2:16B, 3:32B, 4:64B
};

#endif // AXI_TARGET_H
