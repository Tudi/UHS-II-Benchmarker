#pragma once

// page 135
enum REGISTER_ADDRESS_NAMES
{
	RA_Configuration	= 0x000,
	RA_Interrupt		= 0x100,
	RA_Status			= 0x180,
	RA_Command			= 0x200,
};

// page 179
enum REGISTER_ADDRESS_NAMES_CMD
{
	RA_CMD_FULL_RESET	= 0x000,
	RA_CMD_GO_DORMANT	= 0x001,
	RA_CMD_DEVICE_INIT	= 0x002,
	RA_CMD_ENUMERATE	= 0x003,
};