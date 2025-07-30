#include "axi_common.h"
#include "axi_master.h"
#include "axi_target.h"



int sc_main(int argc, char* argv[])
{
    //sc_clock clock("clock", 1, SC_NS); // Create a clock signal with 1 ns period

    AXIMaster axi_master_0("AXI_master_0");
    AXITarget axi_target_0("AXI_target_0");

    // Bind the master and target sockets
    axi_master_0.socket.bind(axi_target_0.socket);

    // Start the simulation
    sc_start(10, SC_NS); // Run the simulation for 10 ns


   return 0;
}