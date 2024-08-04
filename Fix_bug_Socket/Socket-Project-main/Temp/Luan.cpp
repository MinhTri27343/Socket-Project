

//Thread
vector<string> readListFile()
{
    ifstream fin;
    vector<string> list_file;

    fin.open("list_file.txt");
    if (!fin.is_open())
    {
        cout << "Can not open file list_file.txt\n";
        return list_file;
    }
    else
    {
        string temp;
        while (!fin.eof())
        {
            getline(fin, temp, '\n');
            stringstream ss(temp);
            string temp_2;
            getline(ss, temp_2, ' ');
            list_file.push_back(temp_2);
        }
        fin.close();
    }
}
void checkInput(vector<File>& files)
{
    ifstream fin;
    vector<string> list_file = readListFile();
    vector<string> file_need;
    int num_of_line = 0;

    while (true)
    {
        fin.open("input.txt");
        if (!fin.is_open())
        {
            cout << "Can not open file input.txt.\n";
            return;
        }
        string temp;
        int cnt = 0;
        while (cnt++ < num_of_line)
        {
            getline(fin, temp, '\n');
        }
        while (!fin.eof())
        {
            getline(fin, temp, '\n');
            if (temp == "")continue;
            stringstream ss(temp);
            string temp_2;
            getline(ss, temp_2, ' ');
            num_of_line++;
            bool is_have = false;
            for (int i = 0; i < list_file.size(); i++)
            {
                if (list_file[i] == temp_2)is_have = true;
            }
            if (!is_have)
            {
                cout << "Dont have file " << temp_2 << "\n";
                continue;
            }
            bool is_exist = false;
            for (int i = 0; i < file_need.size(); i++)
            {
                if (temp_2 == file_need[i])
                {
                    cout << "File " << temp_2 << " is exist !\n";
                    is_exist = true;
                    break;
                }
            }
            if (is_exist)continue;
            File file_new;

            file_new.file_name = new char[temp_2.length() + 1];
            strcpy_s(file_new.file_name, temp_2.length() + 1, temp_2.c_str());
            file_new.file_name[temp_2.length()] = '\0';
            getline(ss, temp_2, '\n');

            if (temp_2 == "NORMAL") file_new.priority = 1;
            else if (temp_2 == "HIGHT") file_new.priority = 4;
            else if (temp_2 == "CRITICAL") file_new.priority = 10;
            file_need.push_back(file_new.file_name);
            files.push_back(file_new);
        }

        fin.close();
        Sleep(2000);
    }


}
//Display to screen
//
//void gotoxy(unsigned long long x, unsigned long long y)
//{
//	static HANDLE h = NULL;
//	if (!h)
//		h = GetStdHandle(STD_OUTPUT_HANDLE);
//	COORD c = { x, y };
//	SetConsoleCursorPosition(h, c);
//}
//void createBox(unsigned long long x, unsigned long long y, unsigned long long width, unsigned long long height)
//{
//	for (int i = 0; i < width; i++)
//	{
//		gotoxy(x + i + 1, y);
//		cout << HORIZONTAL_LINE;
//		gotoxy(x + i + 1, y + height);
//		cout << HORIZONTAL_LINE;
//	}
//	for (int j = 0; j < height; j++)
//	{
//		gotoxy(x, y + j + 1);
//		cout << VERTICAL_LINE;
//		gotoxy(x + width, y + j + 1);
//		cout << VERTICAL_LINE;
//	}
//	gotoxy(x, y);
//	cout << CORNER_LEFT_TOP;
//	gotoxy(x + width, y);
//	cout << CORNER_RIGHT_TOP;
//	gotoxy(x, y + height);
//	cout << CORNER_LEFT_BOT;
//	gotoxy(x + width, y + height);
//	cout << CORNER_RIGHT_BOT;
//
//}
//void deleteBox(unsigned long long x, unsigned long long y, unsigned long long width, unsigned long long height)
//{
//	for (int i = 0; i < height; i++)
//	{
//		gotoxy(x, y + i);
//		for (int j = 0; j < width; j++)
//		{
//			cout << " ";
//		}
//	}
//}
//void ShowCur(bool CursorVisibility)
//{
//	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
//	CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
//	SetConsoleCursorInfo(handle, &cursor);
//}
