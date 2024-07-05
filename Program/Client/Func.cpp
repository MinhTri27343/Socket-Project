#include"Func.h"
#include"pch.h"
#define HORIZONTAL_LINE char(196)
#define VERTICAL_LINE char(179)
#define CORNER_LEFT_TOP char(218)
#define CORNER_LEFT_BOT char(192)
#define CORNER_RIGHT_BOT char(217)
#define CORNER_RIGHT_TOP char(191)
#include<conio.h>
string normalizeChar(char* source)
{
	string destination = "";
	while (*source)
	{
		destination += source; 
		source++;
	}
	return destination;
}
char* normalizeString(string source)
{
	char* destination = new char[source.size() + 1];
	for (int i = 0; i < source.size(); i++)
	{
		destination[i] = source[i];
	}
	destination[source.size()] = '\0';
	return destination;
}

void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}
void createBox(int x, int y, int width, int height)
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
int getByteSum(string fileName)
{
	ifstream in(fileName, ios::binary);
	in.seekg(0, ios::end);
	int byte_sum = in.tellg();
	return byte_sum;
}
void displayDownload(int coordinate_x, int coordinate_y, string source, int byte_read)
{
	// Fix destination
	string destination = "1" + source;

	// Initialize
	ShowCur(0);
	int width = 26 + source.size();
	int height = 2;
	int percent = 0;
	int byte_sum = getByteSum(source);
	createBox(coordinate_x, coordinate_y, width, height);
	gotoxy(coordinate_x + 1, coordinate_y + 1);
	cout << "Downloading " << source << " .... ";

	// copy file 
	ifstream in(source, ios::binary);
	ofstream out(destination, ios::binary);
	char* read = new char[byte_read];
	while (in.read(read, byte_read))
	{
		percent += byte_read;
		out.write(read, byte_read);
		gotoxy(coordinate_x + 20 + source.size(), coordinate_y + 1);
		cout << (percent * 100) / byte_sum << "%";
		Sleep(100);
	}
	if (in.gcount() > 0)
	{
		in.read(read, in.gcount());
		percent += in.gcount();
		gotoxy(coordinate_x + 20 + source.size(), coordinate_y + 1);
		cout << (percent * 100) / byte_sum << "%";
		_getch();
	}
	delete[] read;
	in.close();
	out.close();

}