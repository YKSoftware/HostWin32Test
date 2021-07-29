#include "stdafx.h"
#include "SharedMemory.h"

constexpr auto MUTEX_NAME = L"HostWin32Test.Mutex01";
constexpr auto SHARED_MEMORY_NAME = L"HostWin32Test.Memory01";
static int sharedMemorySize;
static HANDLE hSharedMemory = NULL;
static HANDLE hMutex = NULL;
static int iCount = 0;

// コンストラクタ
SharedMemory::SharedMemory()
{
}

// デストラクタ
SharedMemory::~SharedMemory()
{
	// 起動数デクリメント
	WaitForSingleObject(hMutex, INFINITE);
	iCount--;
	ReleaseMutex(hMutex);

	if (iCount == 0)
	{
		if (hMutex != NULL)
		{
			CloseHandle(hMutex);
		}
		if ((hSharedMemory != NULL) || (hSharedMemory != INVALID_HANDLE_VALUE))
		{
			UnmapViewOfFile(m_pData);
			CloseHandle(hSharedMemory);
		}
	}
}

bool SharedMemory::TryInitialize(long size)
{
	try
	{
		BOOL bAlreadyExists = true;

		// 排他処理用ミューテックス
		if (hMutex == NULL)
		{
			hMutex = CreateMutex(NULL, FALSE, MUTEX_NAME);
		}

		// ファイルマッピングオブジェクトの作成
		if (hSharedMemory == NULL)
		{
			hSharedMemory = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, HIWORD(size), LOWORD(size), SHARED_MEMORY_NAME);
			bAlreadyExists = (GetLastError() == ERROR_ALREADY_EXISTS);
		}

		// ビューの作成
		m_pData = (byte*)MapViewOfFile(hSharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, size);
		sharedMemorySize = size;

		// 新規に作成された場合は初期化をおこなう
		if (bAlreadyExists == false)
		{
			ZeroMemory(m_pData, size);
		}

		// 起動数インクリメント
		WaitForSingleObject(hMutex, INFINITE);
		iCount++;
		ReleaseMutex(hMutex);

		return true;
	}
	catch (char* str)
	{
		return false;
	}
}

int SharedMemory::ReadToEnd(byte* bytes, int offset)
{
	if ((hSharedMemory == NULL) || (m_pData == NULL))
	{
		return -1;
	}
	if (offset >= sharedMemorySize)
		return -1;
	int length = sharedMemorySize - offset;

	WaitForSingleObject(hMutex, INFINITE);
	memcpy_s(bytes, length, &m_pData[offset], length);
	ReleaseMutex(hMutex);

	return length;
}

int SharedMemory::Read(byte* bytes, int index, int size)
{
	if ((hSharedMemory == NULL) || (m_pData == NULL))
	{
		return -1;
	}
	if (index >= sharedMemorySize)
		return -1;
	int length = (index + size <= sharedMemorySize) ? size : sharedMemorySize - index;

	WaitForSingleObject(hMutex, INFINITE);
	memcpy_s(bytes, length, &m_pData[index], length);
	ReleaseMutex(hMutex);

	return length;
}

int SharedMemory::Write(byte* bytes, int index, int size)
{
	if ((hSharedMemory == NULL) || (m_pData == NULL))
	{
		return -1;
	}
	if (index >= sharedMemorySize)
		return -1;
	int length = (index + size <= sharedMemorySize) ? size : sharedMemorySize - index;

	WaitForSingleObject(hMutex, INFINITE);
	memcpy_s(&m_pData[index], length, bytes, length);
	ReleaseMutex(hMutex);

	return length;
}