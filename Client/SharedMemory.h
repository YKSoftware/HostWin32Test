#pragma once

class SharedMemory
{
public:
	SharedMemory();
	~SharedMemory();

	bool TryInitialize(long size);
	int Read(byte* bytes, int index, int size);
	int Write(byte* bytes, int index, int size);

protected:
	byte* m_pData;
};
