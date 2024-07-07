#include"framework.h"
#include<conio.h>
#include<windows.h>
void SignalCallBack(int signum);
char* normalizeChar(char* source);
unsigned long long getByteSum(string fileName);
char* fileDownload(char* Check, string file_check1, string file_check2, unsigned long long byte_size);
void ShowCur(bool CursorVisibility);
void createBox(unsigned long long x, unsigned long long y, unsigned long long width, unsigned long long height);
void gotoxy(unsigned long long x, unsigned long long y);