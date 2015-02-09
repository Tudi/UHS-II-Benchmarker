#pragma once
#include <windows.h>

class CBitStream
{
public:
	CBitStream(void);
	~CBitStream(void);
	void WriteBits(DWORD value, long nLen=32);
	void ReadBits(DWORD& value, long nLen=32);
	void LoadStream(char * lpszFileName);
	void SaveStream(char * lpszFileName);
	LPBYTE GetStream()	{return m_lpStream;}
	DWORD GetStreamLength()	{return m_dwStreamLen;}
	DWORD GetCurrentPosition()	{return m_dwCurrentPosition;}

private:
	void _WriteBit(BYTE value);
	void _ReadBit(BYTE& value);

private:
	// Private members
	LPBYTE m_lpStream;
	DWORD m_dwStreamLen;
	DWORD m_dwCurrentPosition;
};
