#include <stdio.h>
#include <tchar.h>
#include <windows.h>

DWORD WINAPI ThreadProc(LPVOID lpParam) {
	DWORD* nPtr = (DWORD*)lpParam;
	DWORD numOne = *nPtr;
	DWORD numTwo = *(nPtr + 1);

	DWORD total = 0;
	for (int i = numOne; i <= numTwo;i++)
		total += i;

	return total;
}

int _tmain(int argc, TCHAR* argv[]) {
	DWORD dwThreadID[3];
	HANDLE hThread[3];

	DWORD paramThread[] = { 1,3,4,7,8,10 };
	DWORD total = 0;
	DWORD result = 0;

	for (int i = 0; i < 3; i++) {
		hThread[i] = CreateThread(NULL, 0, ThreadProc, (LPVOID)(&paramThread[i*2]), 0, &dwThreadID[i]);
	}
	if (hThread[0] == NULL || hThread[1] == NULL || hThread[2] == NULL) {
		_tprintf(_T("Thread creation fault! \n"));
		return -1;
	}
	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);
	for (int i = 0; i < 3; i++) {
		GetExitCodeThread(hThread[i], &result);
		total += result;
	}
	_tprintf(_T("total : %d"), total);

	for (int i = 0; i < 3; i++) {
		CloseHandle(hThread[i]);
	}
	return 0;


}