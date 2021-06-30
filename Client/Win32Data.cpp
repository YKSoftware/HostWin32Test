#include "stdafx.h"
#include "Win32Data.h"

int Win32DataMap[E_MAP_MaxValue][2] = {
	{ 4, 0 },			// E_MAP_WindowHandle
	{ 4, 0 },			// E_MAP_WatchDogToWin32,
	{ 4, 0 },			// E_MAP_WindowHandleBack
	{ 4, 0 }			// E_MAP_WatchDogFromWin32
};

#define SIZE	0
#define INDEX	1

// コンストラクタ
Win32Data::Win32Data()
{
	int index = 0;
	for (int i = 0; i < E_MAP_MaxValue; i++)
	{
		Win32DataMap[i][INDEX] = index;
		index += Win32DataMap[i][SIZE];
	}
}

// デストラクタ
Win32Data::~Win32Data()
{
}

int Win32Data::GetSize(Win32DataMapIndexes index)
{
	return Win32DataMap[index][SIZE];
}

int Win32Data::Read(byte* bytes, Win32DataMapIndexes index)
{
	return m_SharedMemory.Read(bytes, Win32DataMap[index][INDEX], Win32DataMap[index][SIZE]);
}

int Win32Data::Write(byte* bytes, Win32DataMapIndexes index)
{
	return m_SharedMemory.Write(bytes, Win32DataMap[index][INDEX], Win32DataMap[index][SIZE]);
}
