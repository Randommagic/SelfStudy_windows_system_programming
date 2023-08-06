#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <tchar.h>
/*
    event와 thread를 둘 다 사용 예제
*/
unsigned int WINAPI OutputThreadFunction(LPVOID);
unsigned int WINAPI CountThreadFunction(LPVOID);

typedef struct _SynchString {
    TCHAR string[100];
    HANDLE hEvent;
    HANDLE hMutex;
} SynchString;

SynchString gSynString;

int _tmain(int argc, TCHAR **argv[]) {
    HANDLE hThreads[2];
    DWORD dwThreadIDs[2];

    gSynString.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    gSynString.hMutex = CreateMutex(NULL, FALSE, NULL);

    if (gSynString.hEvent == NULL || gSynString.hMutex == NULL) {
        _fputts(_T("kernal object creation error\n"), stdout);
        return -1;
    }

    hThreads[0] = (HANDLE)_beginthreadex(NULL, 0, OutputThreadFunction, NULL, 0, (unsigned *)&dwThreadIDs[0]);
    hThreads[1] = (HANDLE)_beginthreadex(NULL, 0, CountThreadFunction, NULL, 0, (unsigned *)&dwThreadIDs[1]);

    if (hThreads[0] == NULL || hThreads[1] == NULL) {
        _fputts(_T("thread creation error\n"), stdout);
        return -1;
    }

    _fputts(_T("Insert String :"), stdout);
    _fgetts(gSynString.string, 30, stdin);

    SetEvent(gSynString.hEvent);
    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

    // 리소스 정리
    CloseHandle(gSynString.hEvent);
    CloseHandle(gSynString.hMutex);
    CloseHandle(hThreads[0]);
    CloseHandle(hThreads[1]);
    return 0;
}

unsigned int WINAPI OutputThreadFunction(LPVOID lpParam) {
    WaitForSingleObject(gSynString.hEvent, INFINITE);
    WaitForSingleObject(gSynString.hMutex, INFINITE);

    _fputts(_T("Output String :"), stdout);
    _fputts(gSynString.string, stdout);

    ReleaseMutex(gSynString.hMutex);
    return 0;
}
unsigned int WINAPI CountThreadFunction(LPVOID lpParam) {
    WaitForSingleObject(gSynString.hEvent, INFINITE);
    WaitForSingleObject(gSynString.hMutex, INFINITE);

    _tprintf(_T("output string length : %d \n"), _tcslen(gSynString.string) - 1);

    ReleaseMutex(gSynString.hMutex);
    return 0;
}