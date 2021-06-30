#include "stdafx.h"
#include "SharedMemory.h"

constexpr auto SHARED_MEMORY_NAME = L"HostWin32Test.Memory01";
constexpr auto MUTEX_NAME = L"HostWin32Test.Mutex01";
constexpr auto SHARED_MEMORY_SIZE = 10 * 1024;
static HANDLE hSharedMemory = NULL;
static HANDLE hMutex = NULL;
static int iCount = 0;


// �R���X�g���N�^
SharedMemory::SharedMemory()
{
	TryInitialize();
}

// �f�X�g���N�^
SharedMemory::~SharedMemory()
{
	// �N�����f�N�������g
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

bool SharedMemory::TryInitialize()
{
	try
	{
		BOOL bAlreadyExists = true;

		// �r�������p�~���[�e�b�N�X
		if (hMutex == NULL)
		{
			hMutex = CreateMutex(NULL, FALSE, MUTEX_NAME);
		}

		// �t�@�C���}�b�s���O�I�u�W�F�N�g�̍쐬
		if (hSharedMemory == NULL)
		{
			hSharedMemory = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, SHARED_MEMORY_SIZE, SHARED_MEMORY_NAME);
			bAlreadyExists = (GetLastError() == ERROR_ALREADY_EXISTS);
		}

		// �r���[�̍쐬
		m_pData = (byte*)MapViewOfFile(hSharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, SHARED_MEMORY_SIZE);

		// �V�K�ɍ쐬���ꂽ�ꍇ�͏������������Ȃ�
		if (bAlreadyExists == false)
		{
			ZeroMemory(m_pData, SHARED_MEMORY_SIZE);
		}

		// �N�����C���N�������g
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

int SharedMemory::Read(byte* bytes, int index, int size)
{
	if ((hSharedMemory == NULL) || (m_pData == NULL))
	{
		if (TryInitialize() == false)
			return -1;
	}
	if (index >= SHARED_MEMORY_SIZE)
		return -1;
	int length = (index + size <= SHARED_MEMORY_SIZE) ? size : SHARED_MEMORY_SIZE - index;

	WaitForSingleObject(hMutex, INFINITE);
	memcpy_s(bytes, length, &m_pData[index], length);
	ReleaseMutex(hMutex);

	return length;
}

int SharedMemory::Write(byte* bytes, int index, int size)
{
	if ((hSharedMemory == NULL) || (m_pData == NULL))
	{
		if (TryInitialize() == false)
			return -1;
	}
	if (index >= SHARED_MEMORY_SIZE)
		return -1;
	int length = (index + size <= SHARED_MEMORY_SIZE) ? size : SHARED_MEMORY_SIZE - index;

	WaitForSingleObject(hMutex, INFINITE);
	memcpy_s(&m_pData[index], length, bytes, length);
	ReleaseMutex(hMutex);

	return length;
}