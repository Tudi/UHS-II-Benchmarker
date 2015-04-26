#pragma once

// set / enable proper build implementations
#define XILINX_PROJECT_BUILD

//standard says 30
#define MAX_PACKET_RESEND_ON_NO_REPLY	5

#define READ_MASK 						0x00000600
#define MAX_WAIT_READ_PACKET_HEADER		( 4 * MICROBLAZE_INSTRUCTION_PER_MILISECOND )
//#define MAX_WAIT_READ_PACKET_HEADER		1

#define DATA_READ_FLAGS_NOT_DATA	0x0100
#define DATA_READ_FLAGS_IS_DATA		0x0600

#define PHY0_PACKET_HEADER0	0x000003BC
#define PHY0_PACKET_HEADER1	0x0000033C

#define PHY0_PACKET_FOOTER0	0x000005BC
#define PHY0_PACKET_FOOTER1	0x000005FD

#define MICROBLAZE_INSTRUCTION_PER_MILISECOND	150000

#define REG_IF_BASEADDR XPAR_REG_IF_BASEADDR
#define USB_BRAM_BASEADDR XPAR_USB_BRAM_CTRL_S_AXI_BASEADDR
#define CONFIG_ADDR (REG_IF_BASEADDR+0x00000000)
#define CONTRL_ADDR (REG_IF_BASEADDR+0x00000010)
#define STATUS_ADDR (REG_IF_BASEADDR+0x00000020)
#define TRANSM_ADDR (REG_IF_BASEADDR+0x00000040)
#define RECEIV_ADDR (REG_IF_BASEADDR+0x00000080)
#define DEB_01_ADDR (REG_IF_BASEADDR+0x00000100)
#define DEB_02_ADDR (REG_IF_BASEADDR+0x00000200)
#define DEB_03_ADDR (REG_IF_BASEADDR+0x00000400)
#define DEB_04_ADDR (REG_IF_BASEADDR+0x00000800)

#ifndef XILINX_PROJECT_BUILD

	#define XPAR_REG_IF_BASEADDR	0

	#ifndef BYTE
		typedef unsigned char BYTE;
	#endif

	#ifndef NULL
		#define NULL 0
	#endif

	#define Xil_Out32( a, b )	1==1;
	#define Xil_In32( a )		PHY0_PACKET_FOOTER1
//	#define Xil_Out32( a, b ) printf( "0x%03 ", b );

	#if !defined( assert )
		#if defined( _DEBUG ) && !defined( XILINX_PROJECT_BUILD )
			#define assert(x) if( !(x) ) *(int*)0 = 1;
		#else
			#define assert(x) 1==1;
		#endif
	#endif

	#include <stdio.h>
	#include "Util.h"
	#include "PacketTypes.h"
	#include "PacketBuilder.h"
	#include "PacketToText.h"
	#include "PacketTimer.h"
	#include "RegisterAddresses.h"
	#include "PacketQueue.h"
	#include "TransactionHandler.h"
	#include "PacketParser.h"

	//contains all test case headers in 1
	#include "TestCases.h"
#else

	#include <stdio.h>
	#include "../../../UHSII_v150420_VTE10A/sources/sw/sim/src/platform.h"
	#include "xil_io.h"
	#include "xparameters.h"
	#include "xbram.h"

	#ifndef BYTE
		typedef unsigned char BYTE;
	#endif

	#ifndef NULL
		#define NULL 0
	#endif

	#include "Util.h"
	#include "PacketTypes.h"
	#include "PacketQueue.h"
	#include "PacketBuilder.h"
	#include "PacketToText.h"
	#include "RegisterAddresses.h"
	#include "TransactionHandler.h"
	#include "PacketParser.h"

	#include "TestCases.h"

	extern u32 config_reg;
	extern u32 contrl_reg;
	extern u32 status_reg;
	extern u32 transm_reg;
	extern u32 receiv_reg;
	extern u32 data_in;
#endif
