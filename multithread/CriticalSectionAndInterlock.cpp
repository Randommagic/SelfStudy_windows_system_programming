#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

#define NUM_OF_GATE 6

LONG gTotalCount = 0;
CRITICAL_SECTION gCriticalSection;

// CriticalSection을 사용한 동기화
void IncreaseCount() {
	EnterCriticalSection(&gCriticalSection);
	gTotalCount++;
	LeaveCriticalSection(&gCriticalSection);
}

// Interlock을 사용한 동기화
void IncreaseCount2() {
	InterlockedIncrement(&gTotalCount);
}

unsigned int WINAPI ThreadProc(LPVOID lpParam) {
	for (DWORD i = 0; i < 1000;i++) {
		//IncreaseCount();
		IncreaseCount2();
	}
	return 0;
}

int _tmain(int argc, TCHAR** argv[]) {
	DWORD dwThreadId[NUM_OF_GATE];
	HANDLE hThread[NUM_OF_GATE];

	InitializeCriticalSection(&gCriticalSection);


	for (DWORD i = 0; i < NUM_OF_GATE;i++) {
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, NULL, CREATE_SUSPENDED, (unsigned*)&dwThreadId[i]);
		if (hThread[i] == NULL) {
			_tprintf(_T("thread creation failed\n"));
			return -1;
		}
	}

	for (DWORD i = 0; i < NUM_OF_GATE;i++)
		ResumeThread(hThread[i]);
	
	WaitForMultipleObjects(NUM_OF_GATE, hThread, TRUE, INFINITE);

	_tprintf(_T("total  count :  %d\n"),gTotalCount);

	for (DWORD i = 0; i < NUM_OF_GATE;i++)
		CloseHandle(hThread[i]);

	DeleteCriticalSection(&gCriticalSection);
	return 0;
}