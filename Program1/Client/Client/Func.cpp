#include"Func.h"
#include"pch.h"
#include<conio.h>
#define HORIZONTAL_LINE char(196)
#define VERTICAL_LINE char(179)
#define CORNER_LEFT_TOP char(218)
#define CORNER_LEFT_BOT char(192)
#define CORNER_RIGHT_BOT char(217)
#define CORNER_RIGHT_TOP char(191)
int cnt_file = 0;
int max_length_file = 0;
void gotoxy(int  x, int  y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}
void createBox(int  x, int  y, int  width, int  height)
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
void deleteBox(int x, int y, int width, int height)
{
	for (int i = 0; i < width; i++)
	{
		gotoxy(x + i, y + height);
		cout << " ";
	}
	for (int i = 0; i < height; i++)
	{
		gotoxy(x, y + i);
		cout << " ";
		gotoxy(x + width, y + i);
		cout << " ";
	}
}
void ShowCur(bool CursorVisibility)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
	SetConsoleCursorInfo(handle, &cursor);
}
void SignalCallBack(int signum) {
	exit(signum);
}
unsigned long long getByteSum(string fileName)
{
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
		return false;
	}
	if (byte_size > 0)
	{
		char* check = new char[byte_size];
		fin.read(check, byte_size);
		string Check = check;
		if (Check.find(file_name) != string::npos)
		{
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
	vector<string> checkDuplicate;
	while (ss >> word)
	{
		if (isFileDownload(file_check1, file_check2, word, byte_size))
		{
			if (!checkDuplicate.empty())
			{
				if (find(checkDuplicate.begin(), checkDuplicate.end(), word) != checkDuplicate.end()) continue;
			}
			checkDuplicate.push_back(word);
			save += word + " ";
		}
	}
	char* a = new char[save.size() + 1];
	strcpy_s(a, save.size() + 1, save.c_str());
	return a;
}
void SendFileNeedDownToServer(CSocket& client, string file_user, int& size_pre_file, int& size_after_file, string file_check1, string file_check2, bool& checkIsExist, char*& send)
{
	ifstream in;
	in.open(file_user, ios::in | ios::binary);
	if (!in.is_open())
	{
		return;
	}
	size_after_file = getByteSum(file_user);
	if (size_after_file > size_pre_file) //Nếu có cập nhật mới
	{
		int byte_size = size_after_file - size_pre_file;
		in.seekg(size_pre_file, ios::beg);
		char* information_file = new char[byte_size + 1];
		in.read(information_file, byte_size);
		information_file[byte_size] = '\0';
		char* temp = normalizeChar(information_file);
		send = fileDownload(temp, file_check1, file_check2, size_pre_file);
		int size_send = strlen(send);
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
void ReceiveInfo1FileFromServer(CSocket& client, int& size_name_file_download, unsigned long long& size_file_download, char*& name_file_download)
{
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

	string file_name = "file.txt";
	ofstream fout;
	fout.open(file_name, ios::out | ios::binary);
	int byte;
	client.Receive((char*)&byte, sizeof(byte), 0);
	char* msg = new char[byte + 1];
	client.Receive(msg, byte, 0);
	msg[byte] = '\0';
	cout << "LIST FILE\n";
	fout << msg;
	stringstream ss(msg);
	string name, size;
	string temp;
	while (getline(ss, temp, '\n'))
	{
		stringstream sss(temp);

		getline(sss, name, ' ');
		cout << "Name: " << name << "\t";
		getline(sss, size, '\n');
		cout << "Size: " << size << "\n";
	}
	

	fout.close();
	delete[] msg;
}
void Receive1FileFromServer(CSocket& client, char* name_file_download, unsigned long long size_file_download, COORD cursorPos)
{
	string name_file_download_str = name_file_download;
	ShowCur(0);
	int height = 2 + cnt_file;
	unsigned long long byte_sum = size_file_download;
	int coordinate_x = 40;
	int coordinate_y = cursorPos.Y + 3;
	deleteBox(coordinate_x, coordinate_y, 30 + max_length_file, height - 1);

	max_length_file = max(max_length_file, name_file_download_str.length());
	int width = 30 + max_length_file;
	createBox(coordinate_x, coordinate_y, width, height);
	gotoxy(coordinate_x + 2, coordinate_y + 1 + cnt_file);
	cout << "Downloading " << name_file_download_str << " .... ";

	ofstream out;
	out.open(("output/" + name_file_download_str).c_str(), ios::trunc | ios::out | ios::binary);
	if (!out)
	{
		cout << "Not open file\n";
		return;
	}
	
	unsigned long long total_byte_curr = 0;
	int byte_read = 32768;

	while (total_byte_curr < size_file_download)
	{
		// receive byte of file download from server  
		char* read_byte_file_download = new char[byte_read];
		int byte_rec = client.Receive(read_byte_file_download, byte_read, 0);
		out.write(read_byte_file_download, byte_rec);
		total_byte_curr = total_byte_curr + byte_rec;

		//=========================================================

		gotoxy(coordinate_x + max_length_file + 25, coordinate_y + 1 + cnt_file);
		cout << (total_byte_curr * 100) / byte_sum << "%";
		cout << endl << endl;
		delete[] read_byte_file_download;
	}
	cnt_file++;
	bool isDone = true;
	client.Send((char*)&isDone, sizeof(isDone));
	delete[] name_file_download;
	out.close();
}
int NumOfFile(char* c)
{
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
COORD getCoordinate()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursorPos;

	if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		cursorPos = csbi.dwCursorPosition;
	}
	else {
		cerr << "Can't get information for buffer of console." << endl;
	}
	return cursorPos;
}
