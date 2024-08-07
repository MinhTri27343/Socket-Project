#pragma once
#include "pch.h"
#include "framework.h"

#define NORMAL 1
#define HIGH 4
#define CRITICAL 10
#define size_buff 32768
using namespace std;

struct File
{
    int size_file_name = 0;
    char* file_name = NULL;
    unsigned long long size_file = 0;
    unsigned long long current_size_file = 0;
    int priority = 0;
};
unsigned long long readSizeFile(string file_name);
bool SendInfoAllFileToClient(CSocket& client);
bool Send1Chunk(CSocket& client, vector<pair<ifstream, File>>& v, int index);
bool isCheckUpdate(CSocket& client, vector<pair<ifstream, File>>& v);
bool SendFileDownloadToClient(CSocket& client, vector<pair<ifstream, File>>& v);
