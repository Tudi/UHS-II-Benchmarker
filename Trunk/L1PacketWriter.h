#ifndef _L1_PACKET_WRITER_H_
#define _L1_PACKET_WRITER_H_

/*
Write a text file that can be read in a human way
*/

struct sL1PacketWriter
{
	FILE	*File;
	char	*FileName;
	int		Lane;	//SD card can have more than 1 data lanes. These need to be configured for Send / Receive mode. Duplex / half duplex
	int		HostID;
	int		DeviceID;
};

/*
Create and initialize a packet writer
*/
sL1PacketWriter *InitL1PacketWriter( char *FName, int pLaneNr, int pHostID, int pDeviceID );
/*
Close files and free memory used by a writer
*/
void DestroyL1PacketWriter( sL1PacketWriter **PW );
/*
Interpret an L1 command. Packetize the command. Write it to a text file
*/
int L0L1ProcessPacket( sL1PacketWriter *PW, const BYTE *Packet );
#endif