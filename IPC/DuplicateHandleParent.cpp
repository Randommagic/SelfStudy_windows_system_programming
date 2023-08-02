#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR *argv[]) {
    HANDLE hProcess;
    TCHAR cmdString[1024];

    DuplicateHandle(GetCurrentProcess(), GetCurrentProcess(), GetCurrentProcess(), &hProcess, 0, TRUE, DUPLICATE_SAME_ACCESS);

    _stprintf(cmdString, _T("%s %u"), _T("DuplicateHandleChild.exe"), (unsigned)hProcess);

    STARTUPINFO si = {
        0,
    };
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = {
        0,
    };

    BOOL isSuccess = CreateProcess(NULL, cmdString, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    if (isSuccess == FALSE) {
        _tprintf(_T("CreateProcess failed\n"));
        return -1;
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    _tprintf(_T("Parent process\n"));
    _tprintf(_T("oops\n"));
    return 0;
}