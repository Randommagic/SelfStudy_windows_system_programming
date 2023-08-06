#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>

#define NUM_OF_CUSTOMER 50
#define RANGE_MIN 10
#define RANGE_MAX 30
#define TABLE_COUNT 10

DWORD randTimeArr[50];
HANDLE hSemaphore;

// Semaphore를 사용한 동기화
void TakeMandu(DWORD time) {
    WaitForSingleObject(hSemaphore, INFINITE);

    _tprintf(_T("eating for %ld mins \n"), time);

    Sleep(100 * time);
    ReleaseSemaphore(hSemaphore, 1, NULL);
}

unsigned int WINAPI ThreadProc(LPVOID lpParam) {
    TakeMandu((DWORD)lpParam);
    return 0;
}

int _tmain(int argc, TCHAR **argv[]) {
    DWORD dwThreadIds[NUM_OF_CUSTOMER];
    HANDLE hThreads[NUM_OF_CUSTOMER];

    // 각 고객 당  시간 랜덤으로 설정
    srand((unsigned)time(NULL));
    for (DWORD i = 0; i < NUM_OF_CUSTOMER; i++) {
        randTimeArr[i] = (DWORD)((double)rand() / (double)RAND_MAX * RANGE_MAX) + RANGE_MIN;
        _tprintf(_T("%ld\n"), randTimeArr[i]);
    }

    // 세마포어 생성 및 초기화
    hSemaphore = CreateSemaphore(NULL, 10, 10, NULL);
    if (hSemaphore == NULL) {
        _tprintf(_T("createSemaphore error: %d\n"), GetLastError());
        return -1;
    }

    // 각각의 고객을 의미하는 쓰레드 생성

    for (DWORD i = 0; i < NUM_OF_CUSTOMER; i++) {
        hThreads[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, (void *)randTimeArr[i], CREATE_SUSPENDED, (unsigned *)&dwThreadIds[i]);
        if (hThreads[i] == NULL) {
            _tprintf(_T("thread creation failed\n"));
            return -1;
        }
    }

    for (DWORD i = 0; i < NUM_OF_CUSTOMER; i++)
        ResumeThread(hThreads[i]);

    WaitForMultipleObjects(NUM_OF_CUSTOMER, hThreads, TRUE, INFINITE);

    _tprintf(_T("-----------END---------"));

    for (DWORD i = 0; i < NUM_OF_CUSTOMER; i++)
        CloseHandle(hThreads[i]);

    CloseHandle(hSemaphore);

    return 0;
}