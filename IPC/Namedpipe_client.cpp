#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define BUF_SIZE 1024

int _tmain(int argc, TCHAR *argv[]) {

    HANDLE hPipe;
    TCHAR readDatabuf[BUF_SIZE + 1];

    while (1) {
        hPipe = CreateFile(_T("\\\\.\\pipe\\simple_pipe"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

        if (hPipe != INVALID_HANDLE_VALUE) {
            break;
        }

        if (GetLastError() != ERROR_PIPE_BUSY) {
            _tprintf(_T("coult  not open pipe\n"));
            return 0;
        }

        if (!WaitNamedPipe(_T("\\\\.\\pipe\\simple_pipe"), 20000)) {
            _tprintf(_T("coult  not open pipe\n"));
            return 0;
        }
    }

    DWORD pipeMode = PIPE_READMODE_MESSAGE | PIPE_WAIT;
    BOOL isSuccess = SetNamedPipeHandleState(hPipe, &pipeMode, NULL, NULL);
    if (!isSuccess) {
        _tprintf(_T("setnamepipehandlestate failed"));
        return 0;
    }

    LPCTSTR fileName = _T("news.txt");
    DWORD bytesWritten = 0;

    isSuccess = WriteFile(hPipe, fileName, (_tcslen(fileName) + 1) * sizeof(TCHAR), &bytesWritten, NULL);
    if (!isSuccess) {
        _tprintf(_T("writefile failed"));
        return 0;
    }

    DWORD bytesRead = 0;
    while (1) {
        isSuccess = ReadFile(hPipe, readDatabuf, BUF_SIZE * sizeof(TCHAR), &bytesRead, NULL);
        if (!isSuccess && GetLastError() != ERROR_MORE_DATA)
            break;
        readDatabuf[bytesRead] = 0;
        _tprintf(_T("%s \n"), readDatabuf);
    }
    CloseHandle(hPipe);
    Sleep(10000);
    return 0;
}
