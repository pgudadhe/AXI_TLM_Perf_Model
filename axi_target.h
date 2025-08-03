#ifndef AXI_TARGET_H
#define AXI_TARGET_H   

#include "axi_common.h"

class AXITarget : public sc_module {
public:
    tlm_utils::simple_target_socket<AXITarget> socket;

    //SC_HAS_PROCESS(AXITarget); 

    AXITarget(sc_module_name name, uint32_t debug_level, uint32_t read_bus_width) : 
            sc_module(name),
            m_cfg_debug_level(debug_level),
            m_cfg_axi_read_bus_width(read_bus_width),
            socket("socket")
    {
        socket.register_nb_transport_fw(this, &AXITarget::nb_transport_fw);

        //SC_METHOD(process_rd_req_method);
        //sensitive << m_process_rd_req_event;
        //dont_initialize();

        //SC_METHOD(process_wr_req_method);
        //sensitive << m_process_wr_req_event;
        //dont_initialize();

        //SC_THREAD(process_rd_req_thread);

        //SC_THREAD(process_wr_req_thread);
    }   

    void end_of_simulation() override {
        cout << "AXITarget: Total Write Delay: " << m_stat_wr_delay << " " << endl;
    }
    tlm_sync_enum nb_transport_fw(tlm_generic_payload& trans, tlm_phase& phase, sc_time& delay) ;
    tlm_sync_enum process_transaction(tlm_generic_payload& trans, tlm_phase& phase, sc_time& delay) ;

private:
    //void process_rd_req_method();
    //sc_event m_process_rd_req_event; // Event to trigger processing of read requests
    //sc_fifo<tlm_generic_payload& > m_rd_req_fifo; // FIFO for read requests
    //void process_rd_req_thread();

    //void process_wr_req_method();
    //sc_event m_process_wr_req_event; // Event to trigger processing of write requests
    //sc_fifo<tlm_generic_payload& > m_wr_req_fifo; // FIFO for write requests
    //void process_wr_req_thread();

    uint32_t m_cfg_axi_read_bus_width = 0; // 0:4B, 1:8B, 2:16B, 3:32B, 4:64B
    uint32_t m_cfg_debug_level = 0; // Debug level

    sc_time m_stat_wr_delay = SC_ZERO_TIME; // Statistics for write delay
};

#endif // AXI_TARGET_H
