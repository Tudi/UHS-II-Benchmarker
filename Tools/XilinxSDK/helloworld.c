/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include "StdAfx.h"

volatile unsigned *module = (unsigned *)0xC3200000;
volatile unsigned *data = (unsigned *)0xC3201000;
volatile unsigned *cfg = (unsigned *)0xC3202000;
volatile unsigned *count = (unsigned *)0xC3203000;
volatile unsigned *data_read = (unsigned *)0xC3204000;
volatile unsigned *st_read = (unsigned *)0xC3205000;
volatile unsigned *count_read = (unsigned *)0xC3206000;

void print(char *str);

#define DisableModuleHandlersAndTranfers() 	module[0] = 0x0;
#define EnableModuleHandlersAndTranfers() 	module[0] = 0x1F;

int main()
{
	volatile int i;
    StatusRegOnRead SR;
    init_platform();

 /*   {
		module[2] = 0x16213; 			// Registrul de configurare statica
		module[1] = 0;					// Registrul de delay
		ResetMemoryToZero();
		data[i] = 0x6541465;			// Scriere date la adresa i
		cfg[i] = 0x04585634;			// Scriere configurare
		module[0] = 0x1F;				// registrul principal este scris si incepe transferul de date catre card
		int AntiDeadlockCounter = 100000000;
		while (module[0] != 0x0000001E && AntiDeadlockCounter > 0 )// Bucla pana cand se termina transferul
			AntiDeadlockCounter--;
		if( AntiDeadlockCounter > 0 )
			xil_printf("there is hope");
		return 0;
    }/**/

	//xil_printf("Status reg( 2 ) : %d\n",module[2]);

//    module[0] = 0x0;	// disable handlers and transfers
    DisableModuleHandlersAndTranfers()

    phy_cmd_type t;
/*    t.uint32_Data = 0;
    t.fields.TDM = 3;
    t.fields.TDRM = 3;
    t.fields.MODE = 1;
//    t.fields.CT_PHY_CMD = 0;
//    t.fields.CT_Rx = 0;
//    t.fields.CT_Tx = 1;
    t.fields.CT = 4;
    t.fields.HOST_MODE = 1;
    t.fields.BUSIF16 = 1;
    t.fields.DET_EN = 1;
    t.fields.RCLKOE = 1;
    t.fields.RCLKTRMEN = 1;
    t.fields.CNFG_ALIGN_EN = 1;
    t.fields.CNFG_LOCK_PERIOD = 0;
    t.fields.CNFG_LOCK_MARGIN = 3;
//    xil_printf("struct : %08X\n", t.uint32_Data);
    */
    t.uint32_Data = 0x0067E09F;

    module[2]= t.uint32_Data;	//setup CFG register to set physical layers in EIDL state


    // Setare asteptare -- 0 adica exista un singur tact de delay intre cand incepe sa scrie si cand incepe sa inregistreze intrarile
    module[1] = 0;

    ResetMemoryToZero();

    //read the status of the lanes
    SR.uint32_Data = module[2];
	xil_printf("1) Status reg( 2 ) RDM: %x %x %x %x\n", SR.fields.RDM,SR.fields.RDTM,SR.fields.ST ,SR.fields.b0  );
	//Sleep( 400 );

	module[0]=0x7ffffffe;

#if 0
/*	t.uint32_Data = 0;
	t.fields.TDM = 3;
	t.fields.TDRM = 3;
	t.fields.MODE = 1;
	t.fields.CT = 4;
	t.fields.HOST_MODE = 1;
	t.fields.BUSIF16 = 1;
	t.fields.DET_EN = 1;
	t.fields.RCLKOE = 1;
	t.fields.RCLKTRMEN = 1;
	t.fields.CNFG_ALIGN_EN = 1;
	t.fields.CNFG_LOCK_PERIOD = 3;
	t.fields.CNFG_LOCK_MARGIN = 3;
//	xil_printf("struct : %08X\n", t.uint32_Data);
	*/
	t.uint32_Data = 0x007FE09F;	//setup CFG register to increase lock period
	module[2] = t.uint32_Data;
	Sleep( 100 );

	//read the new status of the lanes
	SR.uint32_Data = module[2];
	xil_printf("2) Status reg( 2 ) RDM: %x %x %x %x\n", SR.fields.RDM,SR.fields.RDTM,SR.fields.ST ,SR.fields.b0  );
#endif

	i = module[1];
	xil_printf("Status reg( 1 ) : %04x\n", i );
	Sleep( 100 );
/*	t.uint32_Data = 0;
	t.fields.TDM = 3;
	t.fields.TDRM = 3;
	t.fields.MODE = 1;
	t.fields.CT = 0xC;
	t.fields.HOST_MODE = 1;
	t.fields.BUSIF16 = 1;
	t.fields.DET_EN = 1;
	t.fields.RCLKOE = 1;
	t.fields.RCLKTRMEN = 1;
	t.fields.CNFG_ALIGN_EN = 1;
	t.fields.CNFG_LOCK_PERIOD = 3;
	t.fields.CNFG_LOCK_MARGIN = 3;
//	xil_printf("struct : %08X\n", t.uint32_Data);
*/
	t.uint32_Data = 0x007FE19F;	//setup CFG register to set physical layers in STB.L state
	module[2] = t.uint32_Data;
	Sleep( 100 );

	//read the new status of the lanes
	SR.uint32_Data = module[2];
	xil_printf("3) Status reg( 2 ) RDM: %x %x %x %x\n", SR.fields.RDM,SR.fields.RDTM,SR.fields.ST ,SR.fields.b0  );

	SynPacket Synp;
#if 0
	Synp.fields.com = LSS_COM;
	Synp.fields.syn = LSS_SYN0;
	data[0] = FlipBits(  Synp.uint16_Data | ( Synp.uint16_Data << 16 ), 32 );
//	data[0] = Synp.uint16_Data | ( Synp.uint16_Data << 16 );
	count[0] = 20;
	cfg[0] = t.uint32_Data;
#endif
#if 0
	Synp.fields.com = LSS_COM;
	Synp.fields.syn = LSS_SYN0;
	data[0] = Synp.uint16_Data | ( Synp.uint16_Data << 16 );
	count[0] = 20;
	cfg[0] = t.uint32_Data;
#endif
#if 0
	Synp.fields.com = FlipBits( LSS_COM, 8 );
	Synp.fields.syn = FlipBits( LSS_SYN0, 8 );
	data[0] = Synp.uint16_Data | ( Synp.uint16_Data << 16 );
	count[0] = 20;
	cfg[0] = t.uint32_Data;
#endif
#if 0
	Synp.fields.com = FlipBits( LSS_COM, 8 );
	Synp.fields.syn = FlipBits( LSS_SYN0, 8 );
	data[0] = Synp.uint16_Data | ( Synp.uint16_Data << 16 );
	count[0] = 20;
	cfg[0] = 0x04585634;
#endif

	#include "SetData.h"

//	xil_printf("Data 1 is %x\n", data[1]  );
	//start writing
//	module[0] = 0x1F;	// enable handlers and transfers
	EnableModuleHandlersAndTranfers()
//	module[0] = 0xFFFFFFFF;

	//wait until write is finished
	int AntiDeadlockCounter = 10000000;
	while (module[0] != 0x0000001E && AntiDeadlockCounter > 0 )
		AntiDeadlockCounter--;

	if( AntiDeadlockCounter > 0 )
		xil_printf("there is hope");

	xil_printf("count_read = %x\n", count_read[0]);
	xil_printf("st_read = %x\n", st_read[0]);
	xil_printf("data_read = %x\n", data_read[0]);

	xil_printf("data = %x\n", data[0]);
	xil_printf("count = %x\n", count[0]);
	/*
	for(i=0;i<256;i++)
	{
		t.uint32_Data = 0;
		t.fields.TDM = 3;
		t.fields.TDRM = 3;
		t.fields.MODE = 1;
		t.fields.CT = i;
		t.fields.HOST_MODE = 1;
		t.fields.BUSIF16 = 1;
		t.fields.DET_EN = 1;
		t.fields.RCLKOE = 1;
		t.fields.RCLKTRMEN = 1;
		t.fields.CNFG_ALIGN_EN = 1;
		t.fields.CNFG_LOCK_PERIOD = 3;
		t.fields.CNFG_LOCK_MARGIN = 3;

		module[2] = t.uint32_Data;

		Sleep( 100 );

		SR.uint32_Data = module[2];

		if( SR.uint32_Data != 1024 )
			xil_printf("!!!!!!! %d \n", i);
//		xil_printf("Status reg( 2 ) RDM: %d %x %x %x %x\n", i, SR.fields.RDM,SR.fields.RDTM,SR.fields.ST ,SR.fields.b0  );

//		Sleep( 500 );
	}
	*/

    Sleep( 1000 );
    return 0;
}
