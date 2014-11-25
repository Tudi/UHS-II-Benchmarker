#ifndef _L0_PACKET_WRITER_H_
#define _L0_PACKET_WRITER_H_

/*
Write a binary file that can be sent as USH II physical layer communication packets
*/

struct sL0PacketWriter
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
sL0PacketWriter *InitL0PacketWriter( char *FName, int pLaneNr, int pHostID, int pDeviceID );
/*
Close files and free memory used by a writer
*/
void DestroyL0PacketWriter( sL0PacketWriter **PW );
/*
Interpret an L1 command. Packetize the command. Write it to a binary file
*/
int L1L0ProcessLine( sL0PacketWriter *PW, const char *Line );
#endif