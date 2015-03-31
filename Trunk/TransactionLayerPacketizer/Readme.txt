Description
------------
Project will be included into a Xilinx project. It will provide an API for transaction layer packet build / interpret. Will be able to communicate via USB to select test cases and report data back 

Milestones
----------
- Init device
- Config device for read / write
- Do read / write speed test
- Read from usb test case ID. Report back test case 

Usage
----------
- you need to include source files( except StdAfx.h and main.c ) in your Xilinx project
- merge StdAfx.f 
- make sure you compile with XILINX_PROJECT_BUILD defined

Documentation
----------
Right now there is none. Read test cases in this order 
- device init
- device enum
- read register
- set register
- device config
- config device for read
- config device for write


