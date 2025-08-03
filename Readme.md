# AMBA AXI SystemC TLM Approximately Timed Performance Model

## AXI Basics:

In this project we are going to imagine a scenario of a AXI Master and AXI Target module connected by AMBA AXI4 bus. The master could be a hardware accelerator creating a series of Read/Write demands to the target which could be the host memory. I assume that behind the development of this performance model, the intention of the Architect is to find out what would the most optimal bandwidth for Read and Write bus of the AXI data channel. AXI bus consists of multiple channels address, data, response channels for both read and write operations as shown below:


<img width="1364" height="529" alt="image" src="https://github.com/user-attachments/assets/ad957df3-93fc-4580-a45c-ef7fd15378d8" />


Further details can be found at AXI spec which tell about specific signals contained in these buses. For example, Write Address Channel contains awburst, awaddr, awlen, awsize, awready, awvalid signals and buses. But we are not aiming to model a Pin Accurate model of this scenario like an RTL. Our aim is to build an Approximately Timed (AT) Transaction Level Model (TLM) using SystemC for this scenario in order to observe Read/Write throughput in this scanrio by tweaking crucial knobs for this design as we will look ahead in details.

## Building the model:

This project utilizes CMake inorder to build. I assume that your enviornment has been setup using SystemC+WSL+VSCode set up that I have explained in previous post [here](https://pgudadhe.github.io/).
Follow the steps in these articles to build the model.

## Configurations:

This model provides various knobs to configure various scenarios.

```bash
Usage: ./AXI_TLM_PERF_MODEL [options]
Options:
  -h                           Show this help message
  -n <no. of reqs>             Specify number of R/W requests
  -s <request size>            Specify request size in bytes (default 0: 4B to 4096B randomized size), else: Absolute fixed size
  -r <Wr to Rd ratio>          Specify the ratio of Write requests vs. Read. eg. 0.7: 70% Write reqs, 30% Read reqs.
  -d <debug level>             Specify debug level (0-3)
  -rb <AXI Read Bus Width>     Read bus width. 0:4B, 1:8B, 2:16B 3:32B, 4:64B
  -wb <AXI Read Bus Width>     Write bus width. 0:4B, 1:8B, 2:16B 3:32B, 4:64B
Example: ./AXI_TLM_PERF_MODEL -n 10 -r 0.5 -rb 2 -wb 3
This will run the simulation with 10 requests, 50% Write & 50% Read requests,
with Read bus width of 16B and Write bus width of 32B.
Default values: -n 10, -r 0.5, -rb 0, -wb 0
```

| Commandline Option      | Description |
| ----------- | ----------- |
| n | Users can specify the number of requests that need to be made to the AXI target |
| s | Users can keep a fixed size of all requests (upto 4KB) or it can be randomized |
| r | No. of Read requests vs Write requests can be randomized based on probability distribution of Bernoulli |
| rb/wr | Key knobs of this model where user can vary the bandwidth of the Read and Write data buses |

Running the model with default options would show an output like this:

```bash
Info: /OSCI/SystemC: Simulation stopped by user.
Total Read Delay: 32730 ns 
Total Write Delay: 20750 ns
```

## Performance Data Analysis:
I ran multiple experiments on this model collecting data by varying these knobs individually or together. Based on the data I collected, this is the performance analysis I observed. Please note, that the numbers seen here are not real world numbers. The intention here is to observe the trends in data by varying performance model knobs. I used arbitrary values for the delay and cycle time etc. To get more accurate numbers, these values need to be correctly set by consulting the right people. 
Here is the trend observed in the execution time by varying Read and Write bandwidth using -rb and -wb options respectively. 

<img width="752" height="452" alt="image" src="https://github.com/user-attachments/assets/a0e3ffa7-c9ba-483a-88a0-77b64c10747d" />

As expected we can see a great benefit from increasing the bandwidth.

Next, I collected data on execution time by varying the request size. I decided to collect data for these sizes, 64B, 128B, 256B, 512B, 1KB, 2KB and 4KB.

<img width="752" height="452" alt="image" src="https://github.com/user-attachments/assets/3d1b7e49-1308-4dc7-a476-b24387b2d4c4" />

Lastly, I decided to collect data by varying the transaction command probability. Following graph shows how execution time for Read and Write requests varies in accordance with the probability.

<img width="752" height="452" alt="image" src="https://github.com/user-attachments/assets/ebe4d7b1-ddbb-4a2f-b892-93666f96d6a9" />

I plan to add more features to this project like statistics file generation, back pressure etc. 

