# AMBA AXI SystemC TLM Approximately Timed Performance Model

## AXI Basics:

In this project we are going to imagine a scenario of a AXI Master and AXI Target module connected by AMBA AXI4 bus. The master could be a hardware accelerator creating a series of Read/Write demands to the target which could be the host memory. I assume that behind the development of this performance model, the intention of the Architect is to find out what would the most optimal bandwidth for Read and Write bus of the AXI data channel. AXI bus consists of multiple channels address, data, response channels for both read and write operations as shown below:

<img width="1364" height="529" alt="image" src="https://github.com/user-attachments/assets/ad957df3-93fc-4580-a45c-ef7fd15378d8" />

Further details can be found at AXI spec which tell about specific signals contained in these buses. For example, Write Address Channel contains awburst, awaddr, awlen, awsize, awready, awvalid signals and buses. But we are not aiming to model a Pin Accurate model of this scenario like an RTL. Our aim is to build an Approximately Timed (AT) Transaction Level Model (TLM) using SystemC for this scenario in order to observe Read/Write throughput in this scanrio by tweaking crucial knobs for this design as we will look ahead in details.

## Building the model:

This project utilizes CMake inorder to build. I assume that your enviornment has been setup using SystemC+WSL+VSCode set up that I have explained in previous post [here](https://pgudadhe.github.io/).
Follow the steps in these articles to build the model.

## Configurations:

