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

#define DisableModuleHandlersAndTranfers() 	module[0] = 0x0
#define EnableModuleHandlersAndTranfers() 	module[0] = 0x1F
#define SetDelayRegisterNoDelay() 			module[1] = 0
#define DisableDataTransferEnablePINS()		module[0]=0x7ffffffe

int main()
{
	volatile int i;
    StatusRegOnRead SR;
    STField		ST;
    phy_cmd_type0 pct;
    phy_cmd_type t;

    init_platform();

    DisableModuleHandlersAndTranfers();

    // Setare asteptare -- 0 adica exista un singur tact de delay intre cand incepe sa scrie si cand incepe sa inregistreze intrarile
    SetDelayRegisterNoDelay();

    ResetMemoryToZero();
//    xil_printf("struct : %08X\n", t.uint32_Data);

	// let's check if we are in dormant state : lanes are in EIDL state = RDS( 0 ) / RDTS ( 0 )
	 SR.uint32_Data = module[2];
	 ST.uint8_Data = SR.fields.ST;
	 xil_printf("1) Status reg( 2 ) : Amplitude(%x) Lock(%x) Pack(%x) Err(%x) RDS(%x) RDTS(%x)\n", ST.fields.Amplitude, ST.fields.Lock, ST.fields.Pack, ST.fields.Err, ST.fields.RDS, ST.fields.RDTS  );
	
    ////////////////////////////////////////////////////////////////
    //setup CFG register to set physical layers in EIDL state - 0x0067E09F
    // !! this will be visible only once for each time you reset VTE !!!
    {
		t.uint32_Data = 0;
		t.fields.TDM = 3;
		t.fields.TDRM = 3;
		t.fields.MODE = 1;
		t.fields.CT_PHY_CMD = 0;
		t.fields.CT_Tx = 0;	//EIDL
		t.fields.HOST_MODE = 1;
		t.fields.BUSIF16 = 1;
		t.fields.DET_EN = 1;
		t.fields.RCLKOE = 1;
		t.fields.RCLKTRMEN = 1;
		t.fields.CNFG_ALIGN_EN = 1;
		t.fields.CNFG_LOCK_PERIOD = 0;
		t.fields.CNFG_LOCK_MARGIN = 3;
		module[2]= t.uint32_Data;	//setup CFG register to set physical layers in EIDL state - 0x0067E09F

		Sleep( 100 );
		 //read the status of the lanes
		 SR.uint32_Data = module[2];
		 ST.uint8_Data = SR.fields.ST;
	//     xil_printf("1) Status reg( 2 ) RDM: %x %x %x %x\n", SR.fields.RDM,SR.fields.RDTM,SR.fields.ST ,SR.fields.b0  );
		 xil_printf("1) Status reg( 2 ) : Amplitude(%x) Lock(%x) Pack(%x) Err(%x) RDS(%x) RDTS(%x)\n", ST.fields.Amplitude, ST.fields.Lock, ST.fields.Pack, ST.fields.Err, ST.fields.RDS, ST.fields.RDTS  );
    }
    ////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////
    //setup CFG register to set physical layers in STB.L state - 0x0067E09F
    {
		t.uint32_Data = 0;
		t.fields.TDM = 3;
		t.fields.TDRM = 3;
		t.fields.MODE = 1;
		t.fields.CT_PHY_CMD = 7;
		t.fields.CT_Tx = 2; //STB.L
		t.fields.HOST_MODE = 1;
		t.fields.BUSIF16 = 1;
		t.fields.DET_EN = 1;
		t.fields.RCLKOE = 1;
		t.fields.RCLKTRMEN = 1;
		t.fields.CNFG_ALIGN_EN = 1;
		t.fields.CNFG_LOCK_PERIOD = 3;
		t.fields.CNFG_LOCK_MARGIN = 3;
		module[2]= t.uint32_Data;	//setup CFG register to set physical layers in STB.L state - 0x0067E09F

		Sleep( 100 );

		 //read the status of the lanes
		 SR.uint32_Data = module[2];
		 ST.uint8_Data = SR.fields.ST;
		 xil_printf("1) Status reg( 2 ) : Amplitude(%x) Lock(%x) Pack(%x) Err(%x) RDS(%x) RDTS(%x)\n", ST.fields.Amplitude, ST.fields.Lock, ST.fields.Pack, ST.fields.Err, ST.fields.RDS, ST.fields.RDTS  );
    }
    ////////////////////////////////////////////////////////////////
	
	
 	//Sleep( 400 );

 	DisableDataTransferEnablePINS();

 	i = module[1];
 	xil_printf("delay reg( 1 ) : %04x\n", i );
 	Sleep( 100 );

 	Sleep( 100 );
 	pct.uint32_Data = 0;
	pct.fields.TDM = 3;
	pct.fields.TDRM = 3;
	pct.fields.MODE = 1;
	pct.fields.HOST_MODE = 1;
	pct.fields.BUSIF16 = 1;
	pct.fields.DET_EN = 1;
	pct.fields.RCLKOE = 1;
	pct.fields.RCLKTRMEN = 1;
	pct.fields.CNFG_ALIGN_EN = 1;
	pct.fields.CNFG_LOCK_PERIOD = 3;
	pct.fields.CNFG_LOCK_MARGIN = 3;
 	for(pct.fields.CT=0;pct.fields.CT<255;pct.fields.CT++)
 	{
 		//Send out Sync symbol - wait till UHSII device responds with sync byte
		module[2]=pct.uint32_Data;
		Sleep(100);

		//read the new status of the lanes
		SR.uint32_Data = module[2];

		{
			CTFieldMode0 	CT[4];
			STField			ST[4];
			
			xil_printf("\n");
			CT[0].uint8_Data = pct.fields.CT;
			CT[1].fields.CT_PHY_CMD = FlipBits( CT[0].fields.CT_PHY_CMD, 4 );
			CT[1].fields.CT_Tx = FlipBits( CT[0].fields.CT_Tx, 2 );
			CT[1].fields.CT_Rx = FlipBits( CT[0].fields.CT_Rx, 2 );
			CT[2].uint8_Data = FlipBits( pct.fields.CT );
			CT[3].fields.CT_PHY_CMD = FlipBits( CT[2].fields.CT_PHY_CMD, 4 );
			CT[3].fields.CT_Tx = FlipBits( CT[2].fields.CT_Tx, 2 );
			CT[3].fields.CT_Rx = FlipBits( CT[2].fields.CT_Rx, 2 );
			xil_printf("CT = %d - %x - CMD = %x Tx = %x Rx = %x\n", pct.fields.CT, pct.fields.CT, CT[0].fields.CT_PHY_CMD, CT[0].fields.CT_Tx, CT[0].fields.CT_Rx );
			xil_printf("CT = %d - %x - CMD = %x Tx = %x Rx = %x\n", pct.fields.CT, pct.fields.CT, CT[1].fields.CT_PHY_CMD, CT[1].fields.CT_Tx, CT[1].fields.CT_Rx );
			xil_printf("CT = %d - %x - CMD = %x Tx = %x Rx = %x\n", pct.fields.CT, pct.fields.CT, CT[2].fields.CT_PHY_CMD, CT[2].fields.CT_Tx, CT[2].fields.CT_Rx );
			xil_printf("CT = %d - %x - CMD = %x Tx = %x Rx = %x\n", pct.fields.CT, pct.fields.CT, CT[3].fields.CT_PHY_CMD, CT[3].fields.CT_Tx, CT[3].fields.CT_Rx );
			
			xil_printf("ST: %x %x %x %x\n", SR.fields.RDM, SR.fields.RDTM, SR.fields.ST ,SR.fields.b0  );
			
			ST[0].uint8_Data = SR.fields.ST;
			ST[1].uint8_Data = ST[0].uint8_Data;
			ST[1].fields.RDS = FlipBits( ST[0].fields.RDS, 2 );
			ST[1].fields.RDTS = FlipBits( ST[0].fields.RDTS, 2 );
			ST[2].uint8_Data = FlipBits( ST[0].uint8_Data, 8 );
			ST[3].uint8_Data = ST[2].uint8_Data;
			ST[3].fields.RDS = FlipBits( ST[2].fields.RDS, 2 );
			ST[3].fields.RDTS = FlipBits( ST[2].fields.RDTS, 2 );
			xil_printf("ST : Amplitude(%x) Lock(%x) Pack(%x) Err(%x) RDS(%x) RDTS(%x)\n", ST[0].fields.Amplitude, ST[0].fields.Lock, ST[0].fields.Pack, ST[0].fields.Err, ST[0].fields.RDS, ST[0].fields.RDTS  );
			xil_printf("ST : Amplitude(%x) Lock(%x) Pack(%x) Err(%x) RDS(%x) RDTS(%x)\n", ST[1].fields.Amplitude, ST[1].fields.Lock, ST[1].fields.Pack, ST[1].fields.Err, ST[1].fields.RDS, ST[1].fields.RDTS  );
			xil_printf("ST : Amplitude(%x) Lock(%x) Pack(%x) Err(%x) RDS(%x) RDTS(%x)\n", ST[2].fields.Amplitude, ST[2].fields.Lock, ST[2].fields.Pack, ST[2].fields.Err, ST[2].fields.RDS, ST[2].fields.RDTS  );
			xil_printf("ST : Amplitude(%x) Lock(%x) Pack(%x) Err(%x) RDS(%x) RDTS(%x)\n", ST[3].fields.Amplitude, ST[3].fields.Lock, ST[3].fields.Pack, ST[3].fields.Err, ST[3].fields.RDS, ST[3].fields.RDTS  );
			
			if( ( CT[0].CT_Tx == ST[0].fields.RDS && CT[0].CT_Rx == ST[0].fields.RDTS )
				|| ( CT[0].CT_Tx == ST[0].fields.RDTS && CT[0].CT_Rx == ST[0].fields.RDS )

				|| ( CT[1].CT_Tx == ST[1].fields.RDTS && CT[1].CT_Rx == ST[1].fields.RDS )
				|| ( CT[1].CT_Tx == ST[1].fields.RDTS && CT[1].CT_Rx == ST[1].fields.RDS )

				|| ( CT[2].CT_Tx == ST[2].fields.RDTS && CT[2].CT_Rx == ST[2].fields.RDS )
				|| ( CT[2].CT_Tx == ST[2].fields.RDTS && CT[2].CT_Rx == ST[2].fields.RDS )

				|| ( CT[3].CT_Tx == ST[3].fields.RDTS && CT[3].CT_Rx == ST[3].fields.RDS )
				|| ( CT[3].CT_Tx == ST[3].fields.RDTS && CT[3].CT_Rx == ST[3].fields.RDS ) )
					xil_printf( "This is science !\n");
				
		}
 	}

 	module[3]=0xBCBFBCBF;
 	Sleep(100);
 	//read the new status of the lanes
 	SR.uint32_Data = module[2];
 	xil_printf("4) Status reg( 2 ) RDM: %x %x %x %x\n", SR.fields.RDM,SR.fields.RDTM,SR.fields.ST ,SR.fields.b0  );
 	xil_printf("5) data in: %x\n", module[3] );
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

 //	#include "SetData.h"
/*
 //	xil_printf("Data 1 is %x\n", data[1]  );
 	//start writing
 //	module[0] = 0x1F;	// enable handlers and transfers
 	EnableModuleHandlersAndTranfers();
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

 	xil_printf("data = %x %x %x\n", data[0], data[1], data[2]);
 	xil_printf("count = %x %x %x\n", count[0], count[1], count[2]);

 	//read the new status of the lanes
 	SR.uint32_Data = module[2];
 	xil_printf("3) Status reg( 2 ) RDM: %x %x %x %x\n", SR.fields.RDM,SR.fields.RDTM,SR.fields.ST ,SR.fields.b0  );
*/
    Sleep( 1000 );
    return 0;
}
