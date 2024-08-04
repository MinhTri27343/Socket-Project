#pragma once
#include"framework.h"
using namespace std;
void writeFileRecei(string file_recei, char* buffer_receive);
unsigned long long readSizeFile(string file_name);
bool ReadFileNeedDownFromClient(CSocket& client);
bool SendInfo1FileToClient(CSocket& client, string temp_2);
bool Send1FileToClient(CSocket& client, string fileNeedDown);
bool SendInfoAllFileToClient(CSocket& client);
