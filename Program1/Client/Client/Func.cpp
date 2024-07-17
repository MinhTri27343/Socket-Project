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
void SignalCallBack(int signum) {
	cout << "Caught signal " << signum << endl;

	exit(signum);
}
unsigned long long getByteSum(string fileName)
{
	signal(SIGINT, SignalCallBack);
	ifstream in(fileName, ios::binary);
	in.seekg(0, ios::end);
	unsigned long long byte_sum = in.tellg();
	return byte_sum;
}
//Ham dung de dung ctrl+c
char* normalizeChar(char* source)
{
	string s = source;
	while (s[0] == '\n')
	{
		s.erase(0, 1);
	}
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == 'Í')
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
			save += word + " ";
		}
	}
	char* a = new char[save.size() + 1];
	strcpy_s(a, save.size() + 1, save.c_str());
	return a;
}
void SendFileNeedDownToServer(CSocket& client, string file_user, unsigned long long& size_pre_file, unsigned long long& size_after_file, string file_check1, string file_check2, bool& checkIsExist, char*& send)
{
	signal(SIGINT, SignalCallBack);
	ifstream in;
	in.open(file_user, ios::in | ios::binary);
	if (!in.is_open())
	{
		return;
	}
	size_after_file = getByteSum(file_user);
	if (size_after_file > size_pre_file) //Nếu có cập nhật mới
	{
		unsigned long long byte_size = size_after_file - size_pre_file;
		in.seekg(size_pre_file, ios::beg);
		char* information_file = new char[byte_size + 1];
		in.read(information_file, byte_size);
		information_file[byte_size] = '\0';
		char* temp = normalizeChar(information_file);
		send = fileDownload(temp, file_check1, file_check2, size_pre_file);
		unsigned long long size_send = strlen(send);
		if (size_send > 0)
		{
			client.Send((char*)&size_send, sizeof(size_send), 0);
			client.Send(send, size_send, 0);
			checkIsExist = false;
		}
		else
		{
			size_pre_file = size_after_file;
		}
		delete[] information_file;
		delete[] temp;
	}
	in.close();
}
void ReceiveInfo1FileFromServer(CSocket& client, unsigned long long& size_name_file_download, unsigned long long& size_file_download, char*& name_file_download)
{
	signal(SIGINT, SignalCallBack);
	// Ghi nhan so byte cua ten file
	client.Receive((char*)&size_name_file_download, sizeof(size_name_file_download), 0);

	name_file_download = new char[size_name_file_download + 1];
	//Ghi nhan ten file
	client.Receive(name_file_download, size_name_file_download, 0);
	name_file_download[size_name_file_download] = '\0';

	// Ghi nhan byte cua file download 
	client.Receive((char*)&size_file_download, sizeof(size_file_download), 0);
}
void ReceiveInfoAllFileFromServer(CSocket& client)
{
	string file_name = "text1.txt";
	ofstream fout;
	fout.open(file_name, ios::out | ios::binary);
	unsigned long long byte;
	client.Receive((char*)&byte, sizeof(byte), 0);
	char* msg = new char[byte + 1];
	client.Receive(msg, byte, 0);
	msg[byte] = '\0';

	fout << msg;
	cout << msg;

	fout.close();
	delete[] msg;
}
void Receive1FileFromServer(CSocket& client, char* name_file_download, unsigned long long size_file_download)
{
	signal(SIGINT, SignalCallBack);
	string name_file_download_str = name_file_download;
	ShowCur(0);
	unsigned long long width = 26 + name_file_download_str.size();
	unsigned long long height = 2;
	unsigned long long byte_sum = size_file_download;
	unsigned long long coordinate_x = 30;
	unsigned long long coordinate_y = 4;

	createBox(coordinate_x, coordinate_y, width, height);
	gotoxy(coordinate_x + 1, coordinate_y + 1);
	cout << "Downloading " << name_file_download_str << " .... ";


	ofstream out;
	out.open(name_file_download_str.c_str(), ios::app | ios::out | ios::binary);
	unsigned long long total_byte_curr = 0;
	unsigned long long byte_read = 1024;
	while (total_byte_curr < size_file_download)
	{
		// receive byte of file download from server  
		char* read_byte_file_download = new char[byte_read];
		if (size_file_download - total_byte_curr < byte_read)
		{
			client.Receive(read_byte_file_download, size_file_download - total_byte_curr, 0);
			read_byte_file_download[size_file_download - total_byte_curr] = '\0';
			out.write(read_byte_file_download, size_file_download - total_byte_curr);
			total_byte_curr = size_file_download;
		}
		else
		{
			total_byte_curr += byte_read;
			client.Receive(read_byte_file_download, byte_read, 0);
			out.write(read_byte_file_download, byte_read);
		}
		gotoxy(coordinate_x + 20 + name_file_download_str.size(), coordinate_y + 1);
		cout << (total_byte_curr * 100) / byte_sum << "%";
		//Sleep(100); // Sleep for see downloading 
		delete[] read_byte_file_download;
	}
	delete[] name_file_download;
	out.close();
}
int NumOfFile(char* c)
{
	signal(SIGINT, SignalCallBack);
	string str = c;
	int count = 0;
	stringstream ss(str);
	string word;
	while (ss >> word)
	{
		count++;
	}
	return count;
}