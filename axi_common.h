#ifndef AXI_COMMON_H
#define AXI_COMMON_H

#include <iostream>
#include <vector>
#include <string>
#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/peq_with_cb_and_phase.h>

using namespace std;
using namespace sc_core;
using namespace tlm;

#define AXI_CLK_PERIOD 10, SC_NS // Define clock period

#define DEBUG_PRINT(level, msg) \
    if (level <= m_cfg_debug_level) std::cout << msg << std::endl;


#endif // AXI_COMMON_H