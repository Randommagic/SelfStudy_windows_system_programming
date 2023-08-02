#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR *argv[]) {
    HANDLE hReadPipe, hWritePipe;
    TCHAR sendString[] = _T("anonymous  pipe");
    TCHAR recvString[100];

    DWORD bytesWritten, bytesRead;

    // create pipe
    CreatePipe(&hReadPipe, &hWritePipe, NULL, 0);
    // LPSECURITY_ATTRIBUTES la;
    // la->bInheritHandle = TRUE;
    // la->nLength = sizeof(la);

    // 데이터 송신
    WriteFile(hWritePipe, sendString, lstrlen(sendString) * sizeof(TCHAR), &bytesWritten, NULL);
    _tprintf(_T("string sent : %s\n"), sendString);

    ReadFile(hReadPipe, recvString, bytesWritten, &bytesRead, NULL);
    recvString[bytesRead / sizeof(TCHAR)] = 0;

    _tprintf(_T("string recv : %s\n"), recvString);

    CloseHandle(hReadPipe);
    CloseHandle(hWritePipe);

    return 0;
}