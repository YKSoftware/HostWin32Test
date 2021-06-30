#pragma once

// CClientDlg ダイアログ
class SharedMemory
{
public:
	SharedMemory();
	~SharedMemory();

	int Read(byte* bytes, int index, int size);
	int Write(byte* bytes, int index, int size);

protected:
	bool TryInitialize();
	byte* m_pData;
};
