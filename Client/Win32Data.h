#pragma once
#include "SharedMemory.h"

typedef enum
{
	E_MAP_WindowHandle = 0,
	E_MAP_CreateWindowCommand,

	E_MAP_CreateWindowAnswer,
	E_MAP_MaxValue
} Win32DataMapIndexes;

class Win32Data
{
public:
	Win32Data();
	~Win32Data();

	long GetTotalSize();
	int GetSize(Win32DataMapIndexes index);
	int ReadToEnd(byte* bytes, int offset = 0);
	int Read(byte* bytes, Win32DataMapIndexes index);
	int Write(byte* bytes, Win32DataMapIndexes index);

protected:
	SharedMemory m_SharedMemory;
	long m_MemorySize;
};
