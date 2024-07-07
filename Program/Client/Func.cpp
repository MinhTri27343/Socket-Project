#include"Func.h"
#include"pch.h"
#include<conio.h>
#define HORIZONTAL_LINE char(196)
#define VERTICAL_LINE char(179)
#define CORNER_LEFT_TOP char(218)
#define CORNER_LEFT_BOT char(192)
#define CORNER_RIGHT_BOT char(217)
#define CORNER_RIGHT_TOP char(191)
void gotoxy(unsigned long long x, unsigned long long y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}
void createBox(unsigned long long x, unsigned long long y, unsigned long long width, unsigned long long height)
{

	for (int i = 0; i < width; i++)
	{
		gotoxy(x + i + 1, y);
		cout << HORIZONTAL_LINE;
		gotoxy(x + i + 1, y + height);
		cout << HORIZONTAL_LINE;
	}
	for (int j = 0; j < height; j++)
	{
		gotoxy(x, y + j + 1);
		cout << VERTICAL_LINE;
		gotoxy(x + width, y + j + 1);
		cout << VERTICAL_LINE;
	}
	gotoxy(x, y);
	cout << CORNER_LEFT_TOP;
	gotoxy(x + width, y);
	cout << CORNER_RIGHT_TOP;
	gotoxy(x, y + height);
	cout << CORNER_LEFT_BOT;
	gotoxy(x + width, y + height);
	cout << CORNER_RIGHT_BOT;

}
void ShowCur(bool CursorVisibility)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
	SetConsoleCursorInfo(handle, &cursor);
}
unsigned long long getByteSum(string fileName)
{
	ifstream in(fileName, ios::binary);
	in.seekg(0, ios::end);
	unsigned long long byte_sum = in.tellg();
	return byte_sum;
}
//Ham dung de dung ctrl+c
void SignalCallBack(int signum) {
	cout << "Caught signal " << signum << endl;

	exit(signum);
}
char* normalizeChar(char* source)
{
	string s = source;
	while (s[0] == '\n')
	{
		s.erase(0, 1);
	}
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == '�')
		{
			s.erase(i, 1);
			i--;
		}
		if (s[i] == '\n')
		{
			s[i] = ' ';
		}
	}
	char* a = new char[s.size() + 1];
	strcpy_s(a, s.size() + 1, s.c_str());
	return a;
}
bool isFileDownload(string file_check1, string file_check2, string file_name, unsigned long long byte_size)
{
	// Condition1: Is file_name exist in file input.txt ?
	ifstream fin(file_check1, ios::binary);
	if (!fin)
	{
		cout << "Read file error\n";
		return false;
	}
	if (byte_size > 0)
	{
		char* check = new char[byte_size];
		fin.read(check, byte_size);
		string Check = check;
		if (Check.find(file_name) != string::npos)
		{
			cout << "File is exist \n";
			fin.close();
			return false;
		}
		delete[] check;
		fin.close();
	}

	// Condition2: Has file_name in list_file ?  
	ifstream in(file_check2, ios::in);
	if (!in)
	{
		cout << "Read file error\n";
		return false;
	}
	string word;
	while (getline(in, word))
	{
		if (word.find(file_name) != string::npos)
		{
			fin.close();
			return true;
		}
	}
	in.close();
	return false;

}
char* fileDownload(char* Check, string file_check1, string file_check2, unsigned long long byte_size)
{
	string check = Check;
	stringstream ss(check);
	string word;
	string save = "";
	while (ss >> word)
	{
		if (isFileDownload(file_check1, file_check2, word, byte_size))
		{
			save += word;
		}
	}
	char* a = new char[save.size() + 1];
	strcpy_s(a, save.size() + 1, save.c_str());
	return a;
}