#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR *argv[]) {
    HANDLE hParent = (HANDLE)_ttoi(argv[1]);
    DWORD isSuccess = WaitForSingleObject(hParent, INFINITE);

    _tprintf(_T("Child process\n"));

    if (isSuccess == WAIT_FAILED) {
        _tprintf(_T("Wait_Failed returned\n"));
        Sleep(10000);
        return -1;
    } else {
        _tprintf(_T("ok it worked \n"));
        Sleep(10000);
        return 0;
    }
    return 0;
}