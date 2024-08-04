#pragma once
#include"framework.h"
#include "pch.h"

using namespace std;
#define NORMAL 1
#define HIGH 4
#define CRITICAL 10
#define size_buff 40960
struct File
{
    int size_file_name = 0;
    char* file_name = NULL;
    unsigned long long size_file = 0;
    unsigned long long current_size_file = 0;
    int priority = 0;
};
void ReceiveInfoAllFileFromServer(CSocket& client);
void gotoxy(unsigned long long x, unsigned long long y);
void Receive1Chunk(CSocket& client, vector<pair<ofstream, File>>& v, int index);
void checkIsUpdate(CSocket& client, vector<pair<ofstream, File>>& v, vector<File>& tmp);
void ReceiveFileDownloadToClient(CSocket& client, vector<pair<ofstream, File>>& v, vector<File>& tmp);
void checkInput(vector<File>& files, vector<string>list_file);
vector<string> readListFile();
void SignalCallBack(int signum);
