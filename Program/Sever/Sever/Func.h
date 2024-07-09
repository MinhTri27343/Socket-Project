#pragma once
#include"framework.h"
using namespace std;
void writeFileRecei(string file_recei, char* buffer_receive);
unsigned long long readSizeFile(string file_name);
void ReadFileNeedDownFromClient(CSocket& client);
void SendInfo1FileToClient(CSocket& client, string temp_2);
void Send1FileToClient(CSocket& client, string fileNeedDown);
void SendInfoAllFileToClient(CSocket& client);
