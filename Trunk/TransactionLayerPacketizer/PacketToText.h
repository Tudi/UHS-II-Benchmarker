#pragma once

/*
Format ccmd header to human readable format
*/
void FormatToTextCCMD( TLPU_CCMD *P_CCMD );
/*
Format ccmd-resp header to human readable format
*/
void FormatToTextCCMDResp( TLPU_RES *P_RES );
/*
Print out in human readable format the roundtrip times of a transaction
*/
void FormatToTextTransactionRoundTripTimes( int TransactionId );