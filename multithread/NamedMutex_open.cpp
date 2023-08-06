#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <tchar.h>

HANDLE hMutex;
DWORD dwWaitResult;

// CriticalSection을 사용한 동기화
void ProcessBaseCriticalSection() {
    dwWaitResult = WaitForSingleObject(hMutex, INFINITE);

    switch (dwWaitResult) {
    case WAIT_OBJECT_0:
        break;
    case WAIT_TIMEOUT:
        break;
    case WAIT_ABANDONED:
        break;
    }

    for (DWORD i = 0; i < 5; i++) {
        _tprintf(_T("Thread Running!"));
        Sleep(3000);
    }

    ReleaseMutex(hMutex);
}

int _tmain(int argc, TCHAR **argv[]) {

#if 0
    hMutex = CreateMutex(NULL, FALSE, _T("NamedMutex"));
#else
    hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, _T("NamedMutex"));
#endif

    if (hMutex == NULL) {
        _tprintf(_T("CreateMutex error : %d \n"), GetLastError());
        return -1;
    }
    ProcessBaseCriticalSection();
    CloseHandle(hMutex);
    return 0;
}