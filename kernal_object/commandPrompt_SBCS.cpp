#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <windows.h>

#define STR_LEN 256
#define CMD_TOKEN_NUM 10

int CmdReadTokenize(void);
int CmdProcessing(int);
char *StrLower(char *);

char cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
char cmdString[STR_LEN];
char seps[] = " ,\t\n";

int main(int argc, char *argv[]) {

    setlocale(LC_ALL, "Korean");

    if (argc > 2) {
        for (int i = 1; i < argc; i++)
            strcpy(cmdTokenList[i - 1], argv[i]);
    }
    int IsExit = 0;

    while (true) {
        int tokenNum = CmdReadTokenize();
        if (tokenNum == 0)
            continue;
        IsExit = CmdProcessing(tokenNum);
        if (IsExit == TRUE) {
            fputs("명령어 처리를 종료합니당\n", stdout);
            break;
        }
    }
}

int CmdReadTokenize(void) {
    char *token;
    fputs("best commandprompts >> ", stdout);
    fgets(cmdString, CMD_TOKEN_NUM, stdin); // _fgetts deleted

    token = strtok(cmdString, seps);
    int tokenNum = 0;
    while (token != NULL) {
        strcpy(cmdTokenList[tokenNum++], StrLower(token));
        token = strtok(NULL, seps);
    }
    return tokenNum;
}

int CmdProcessing(int tokenNum) {
    BOOL isRun;
    STARTUPINFOA si = {
        0,
    };
    PROCESS_INFORMATION pi;
    si.cb = sizeof(si);

    char cmdStringWithOptions[STR_LEN] = {
        0,
    };
    char optString[STR_LEN] = {
        0,
    };

    if (!strcmp(cmdTokenList[0], "exit")) {
        return TRUE;
    } else if (!strcmp(cmdTokenList[0], "start")) { // input 'start ~~'
        if (tokenNum > 1) {
            for (int i = 1; i < tokenNum; i++)
                sprintf(optString, "%s %s", optString, cmdTokenList[i]);
            sprintf(cmdStringWithOptions, "%s %s", "cmd.exe", optString);
        } else { // only start
            sprintf(cmdStringWithOptions, "%s", "cmd.exe");
        }
        // cmdStringWithOptions is not wchar, so using createProcessA instead of
        // createProcess createProcessA requires STARTUPINFOA instead of
        // STARTUPINFO
        isRun = CreateProcessA(NULL, cmdStringWithOptions, NULL, NULL, TRUE,
                               CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else if (!strcmp(cmdTokenList[0], "echo")) {
        for (int i = 1; i < tokenNum; i++)
            sprintf(optString, "%s %s", optString, cmdTokenList[i]);
        printf("echo message : %s", optString);
    } else {
        strcpy(cmdStringWithOptions, cmdTokenList[0]);
        for (int i = 1; i < tokenNum; i++)
            sprintf(cmdStringWithOptions, "%s %s", cmdStringWithOptions,
                    cmdTokenList[i]);
        isRun = CreateProcessA(NULL, cmdStringWithOptions, NULL, NULL, TRUE, 0,
                               NULL, NULL, &si, &pi);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        if (isRun == FALSE) {
            printf("error\n");
        }
    }
    return 0;
}

char *StrLower(char *pStr) {
    char *ret = pStr;
    while (*pStr) {
        if (isupper(*pStr))
            *pStr = tolower(*pStr);
        pStr++;
    }
    return ret;
}