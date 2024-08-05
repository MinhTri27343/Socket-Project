#pragma once
#include"framework.h"
#include "pch.h"

using namespace std;
#define NORMAL 1
#define HIGH 4
#define CRITICAL 10
#define size_buff 32768
struct File
{
    int size_file_name = 0;
    char* file_name = NULL;
    unsigned long long size_file = 0;
    unsigned long long current_size_file = 0;
    int priority = 0;
};
void ReceiveInfoAllFileFromServer(CSocket& client);
void Receive1Chunk(CSocket& client, vector<pair<ofstream, File>>& v, int index);
void checkIsUpdate(CSocket& client, vector<pair<ofstream, File>>& v, vector<File>& tmp);
void ReceiveFileDownloadToClient(CSocket& client, vector<pair<ofstream, File>>& v, vector<File>& tmp, int width_max);
void checkInput(vector<File>& files, vector<string>list_file);
vector<string> readListFile();
void SignalCallBack(int signum);
//======================Display============================
void gotoxy(int x, int y);
void createBox(int  x, int  y, int  width, int  height);
void deleteContent(int x, int y, int width, int height);
void ShowCur(bool CursorVisibility);
void displayPercent(int& number_of_file, vector<File> files, int& width_max);
//=====
COORD getCoordinate();