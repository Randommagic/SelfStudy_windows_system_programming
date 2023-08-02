#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define BUF_SIZE 1024
int CommToClient(HANDLE);

int _tmain(int argc, TCHAR *argv[]) {
    LPTSTR pipeName = _T("\\\\.\\pipe\\simple_pipe");
    HANDLE hPipe;

    while (1) {
        hPipe = CreateNamedPipe(pipeName, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, BUF_SIZE,
                                BUF_SIZE, 20000, NULL);
        if (hPipe == INVALID_HANDLE_VALUE) {
            _tprintf(_T("CreatePipe failed"));
            return -1;
        }
        BOOL isSuccess = 0;
        isSuccess = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (isSuccess)
            CommToClient(hPipe);
        else
            CloseHandle(hPipe);
    }

    return 1;
}

int CommToClient(HANDLE hPipe) {
    TCHAR fileName[MAX_PATH];
    TCHAR dataBuf[BUF_SIZE];

    BOOL isSuccess;
    DWORD fileNameSize;
    isSuccess = ReadFile(hPipe, fileName, MAX_PATH * sizeof(TCHAR), &fileNameSize, NULL);

    if (!isSuccess || fileNameSize == 0) {
        _tprintf(_T("pipe read message failed"));
        return -1;
    }

    FILE *filePtr = _tfopen(fileName, _T("r"));
    if (filePtr == NULL) {
        _tprintf(_T("File open fault!\n"));
        return -1;
    }

    DWORD bytesWritten = 0;
    DWORD bytesRead = 0;
    while (!feof(filePtr)) {
        bytesRead = fread(dataBuf, 1, BUF_SIZE, filePtr);
        WriteFile(hPipe, dataBuf, bytesRead, &bytesWritten, NULL);
        if (bytesRead != bytesWritten) {
            _tprintf(_T("pipe write  message error \n"));
            break;
        }
    }
    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    return 1;
}