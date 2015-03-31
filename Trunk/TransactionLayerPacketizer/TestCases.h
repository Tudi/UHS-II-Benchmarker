#pragma once

/*
Send device Init and Device enum packets
*/
void TestCaseDeviceInit();
/*
Read Device register values into "DeviceState" variable
*/
void TestCaseDeviceCapabilities();
/*
Write max_blk len register value to the device
*/
void TestCaseDeviceSetRegister();
/*
This is how the device was tested and it was confirmed to be working. Will be used a software self test sample
*/
void TestCaseReproduceDanielExample();
