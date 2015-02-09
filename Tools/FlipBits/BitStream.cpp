#include "BitStream.h"
#include <stdio.h>
#include <io.h>
#include <stdlib.h>


CBitStream::CBitStream(void)
{
	m_lpStream = NULL;
	m_dwStreamLen = 0;
	m_dwCurrentPosition = 0;
}

CBitStream::~CBitStream(void)
{
	// Free stream
	if (m_lpStream != NULL)
	{
		free(m_lpStream);
		m_lpStream = NULL;
		m_dwStreamLen = 0;
	}
}

void CBitStream::_WriteBit(BYTE value)
{
	if( m_dwCurrentPosition >= m_dwStreamLen )
	{
		m_dwStreamLen += 8;
		m_lpStream = (LPBYTE)realloc( m_lpStream, ( m_dwStreamLen / 8 + 1 ) * sizeof(BYTE));
		m_lpStream[ ( m_dwStreamLen - 1 ) / 8 ] = 0;
	}

	DWORD currentByte = m_dwCurrentPosition >> 3;
	BYTE currentBit = (BYTE)(m_dwCurrentPosition % 8);
	m_lpStream[currentByte] |= ( value << ( 7 - currentBit ) );
	m_dwCurrentPosition++;

}

void CBitStream::_ReadBit(BYTE& value)
{
	// Check for valid position
	value = 0x00;
	if (m_dwCurrentPosition < m_dwStreamLen)
	{
		// Read single BIT
		DWORD currentByte = m_dwCurrentPosition >> 3;
		BYTE currentBit = (BYTE)(m_dwCurrentPosition % 8);
		value = ((BYTE)(m_lpStream[currentByte] >> ( 7 - currentBit)));
		m_dwCurrentPosition++;
	}
}

void CBitStream::WriteBits(DWORD value, long nLen)
{
	// Write single BITs
	long _nLen = max(0, min(32, nLen));
	for (long i=0; i<_nLen; i++)
	{
		BYTE bitValue = ( value >> ( _nLen - 1 - i ) ) & 1;
		_WriteBit(bitValue);
	}
}

void CBitStream::ReadBits(DWORD& value, long nLen)
{
	// Read single BITs
	value = 0x00000000;
	DWORD _nLen = max(0, min(32, nLen));
	for (DWORD i=0; i<_nLen; i++)
	{
		BYTE bitValue;
		_ReadBit( bitValue );
		value |= (bitValue << ( nLen - 1 - i ));
	}
}

void CBitStream::LoadStream(char * lpszFileName)
{
	// Check for valid file name
	if (lpszFileName != NULL)
	{
		// Free stream
		if (m_lpStream != NULL)
		{
			free(m_lpStream);
			m_lpStream = NULL;
			m_dwStreamLen = 0;
		}

		// Read stream from file
		FILE* file = fopen(lpszFileName, ("rb"));
		if (file != NULL)
		{
			m_dwStreamLen = _filelength(file->_file);
			m_lpStream = (LPBYTE)malloc(m_dwStreamLen*sizeof(BYTE));
			fread(m_lpStream, sizeof(BYTE), m_dwStreamLen, file);
			fclose(file);
			m_dwStreamLen = ( m_dwStreamLen - 0 ) << 3;
			m_dwCurrentPosition = 0;
		}
	}
}

void CBitStream::SaveStream(char * lpszFileName)
{
	// Check for valid file name
	if (lpszFileName != NULL)
	{
		// Check for valid stream
		if (m_lpStream != NULL)
		{
			// Write stream to file
			FILE* file = fopen(lpszFileName, ("wb"));
			if (file != NULL)
			{
				fwrite(m_lpStream, sizeof(BYTE), m_dwStreamLen / 8, file);
				fclose(file);
			}
		}
	}
}
