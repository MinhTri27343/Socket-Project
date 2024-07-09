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
void SendFileNeedDownToServer(CSocket& client, string file_user, unsigned long long& size_pre_file, unsigned long long& size_after_file, string file_check1, string file_check2, bool& checkIsExist, char*& send);
void ReceiveInfo1FileFromServer(CSocket& client, unsigned long long& size_name_file_download, unsigned long long& size_file_download, char*& name_file_download);
void ReceiveInfoAllFileFromServer(CSocket& client);
void Receive1FileFromServer(CSocket& client, char* name_file_download, unsigned long long size_file_download);
int NumOfFile(char* c);
