#pragma once

/*
Start a timer in the moment we sent a packet to the physical layer
*/
void PacketSendStartTimer( struct TransactionLayerPacket *Packet );
/*
Stop the timer in the moment we received a packet from the physical layer
*/
void PacketRecvStopTimer( struct TransactionLayerPacket *Packet );
/*
Get the duration of roundtrips for a specific transaction 
*/
int GetTransactionDuration( int TransactionId );