#include "axi_common.h"
#include "axi_master.h"
#include "axi_target.h"

string g_cfg_state_filename = "";
uint32_t g_cfg_no_of_reqs = 10;
float g_cfg_wr_to_rd_ratio = 0.5f;      // 50% Write
uint32_t g_cfg_axi_read_bus_width = 0;  // 0:4B, 1:8B, 2:16B, 3:32B, 4:64B
uint32_t g_cfg_axi_write_bus_width = 0; // 0:4B, 1:8B, 2:16B, 3:32B, 4:64B
uint32_t g_cfg_debug_level = 0;         // Default debug level
uint32_t g_cfg_req_size = 0;            // Default request size in bytes

int parseOptions(int argc, char **argv)
{
    if (argc < 2)
    {
        return 0; // No options provided, use defaults
    }
    if (!strcmp(argv[1], "-h"))
    {
        cout << "Usage: " << argv[0] << " [options]\n";
        cout << "Options:\n";
        cout << "  -h                           Show this help message\n";
        cout << "  -stat <stats_file>           Specify stats output file\n";
        cout << "  -n <no. of reqs>             Specify number of R/W requests\n";
        cout << "  -s <request size>            Specify request size in bytes (default 0: 4B to 4096B randomized size), else: Absolute fixed size\n";
        cout << "  -r <Wr to Rd ratio>          Specify the ratio of Write requests vs. Read. eg. 0.7: 70% Write reqs, 30% Read reqs.\n";
        cout << "  -d <debug level>             Specify debug level (0-3)\n";
        cout << "  -rb <AXI Read Bus Width>     Read bus width. 0:4B, 1:8B, 2:16B 3:32B, 4:64B\n";
        cout << "  -wb <AXI Read Bus Width>     Write bus width. 0:4B, 1:8B, 2:16B 3:32B, 4:64B\n";
        cout << "Example: " << argv[0] << " -n 10 -r 0.5 -rb 2 -wb 3\n";
        cout << "This will run the simulation with 10 requests, 50% Write and 50% Read requests, with Read bus width of 16B and Write bus width of 32B.\n";
        cout << "Default values: -n 10, -r 0.5, -rb 0, -wb 0\n";
        cout << "Note: The -stat option is not implemented yet.\n";
        cout << endl;
        return 1;
    }

    *argv++;
    argc--;

    // Process command line arguments
    while (argc > 0)
    {
        if (strcmp(*argv, "-stat") == 0)
        {
            *argv++;
            argc--;
            g_cfg_state_filename = *argv;
        }
        if (strcmp(*argv, "-n") == 0)
        {
            *argv++;
            argc--;
            g_cfg_no_of_reqs = atoi(*argv);
        }
        if (strcmp(*argv, "-r") == 0)
        {
            *argv++;
            argc--;
            g_cfg_wr_to_rd_ratio = atof(*argv);
        }
        if (strcmp(*argv, "-s") == 0)
        {
            *argv++;
            argc--;
            g_cfg_req_size = atoi(*argv);
            if (g_cfg_req_size < 0 || g_cfg_req_size > 4096)
            {
                cout << "Invalid request size. Must be between 0 and 4096 bytes." << endl;
                return 1;
            }
        }
        if (strcmp(*argv, "-d") == 0)
        {
            *argv++;
            argc--;
            g_cfg_debug_level = atoi(*argv);
        }
        if (strcmp(*argv, "-rb") == 0)
        {
            *argv++;
            argc--;
            g_cfg_axi_read_bus_width = atoi(*argv);
        }
        if (strcmp(*argv, "-wb") == 0)
        {
            *argv++;
            argc--;
            g_cfg_axi_write_bus_width = atoi(*argv);
        }

        *argv++;
        argc--;
    }

    return 0;
}

int sc_main(int argc, char *argv[])
{
    if (parseOptions(argc, argv) != 0)
    {
        return 1; // Error in parsing options
    }
    cout << "AXI TLM Performance Model\n";
    cout << "Number of requests: " << g_cfg_no_of_reqs << endl;
    cout << "Write to Read ratio: " << g_cfg_wr_to_rd_ratio << endl;
    cout << "AXI Read Bus Width: " << g_cfg_axi_read_bus_width << endl;
    cout << "AXI Write Bus Width: " << g_cfg_axi_write_bus_width << endl;
    cout << "Debug Level: " << g_cfg_debug_level << endl;
    cout << "Request Size: " << g_cfg_req_size << " bytes" << endl;
    
    AXIMaster axi_master_0("AXI_master_0",
                           g_cfg_debug_level,
                           g_cfg_no_of_reqs,
                           g_cfg_req_size,
                           g_cfg_wr_to_rd_ratio,
                           g_cfg_axi_write_bus_width);
    AXITarget axi_target_0("AXI_target_0",
                           g_cfg_debug_level,
                           g_cfg_axi_read_bus_width);

    // Bind the master and target sockets
    axi_master_0.socket.bind(axi_target_0.socket);

    // Start the simulation
    sc_start(10000, SC_NS); // Run the simulation for 10 ms
    
    sc_stop(); // Stop the simulation

    return 0;
}