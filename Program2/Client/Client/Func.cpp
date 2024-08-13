#include"Func.h"
#include "pch.h"
#include"framework.h"
using namespace std;
#define HORIZONTAL_LINE char(196)
#define VERTICAL_LINE char(179)
#define CORNER_LEFT_TOP char(218)
#define CORNER_LEFT_BOT char(192)
#define CORNER_RIGHT_BOT char(217)
#define CORNER_RIGHT_TOP char(191)
std::mutex mtx;
int size_pre = 0;
void SignalCallBack(int signum) {
    COORD pos = getCoordinate();
    gotoxy(0, pos.Y + 10);
    exit(signum);
}

//=======================Display=============================================
void gotoxy(int x, int y)
{
    static HANDLE h = NULL;
    if (!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { x, y };
    SetConsoleCursorPosition(h, c);
}
void createBox(int  x, int  y, int  width, int  height)
{
    for (int i = 0; i < width + 1; i++)
    {
        gotoxy(x + i + 1, y);
        cout << HORIZONTAL_LINE;
        gotoxy(x + i + 1, y + height + 1);
        cout << HORIZONTAL_LINE;
    }
    for (int j = 0; j < height + 1; j++)
    {
        gotoxy(x, y + j + 1);
        cout << VERTICAL_LINE;
        gotoxy(x + width + 1, y + j + 1);
        cout << VERTICAL_LINE;
    }
    gotoxy(x, y);
    cout << CORNER_LEFT_TOP;
    gotoxy(x + width + 1, y);
    cout << CORNER_RIGHT_TOP;
    gotoxy(x, y + height + 1);
    cout << CORNER_LEFT_BOT;
    gotoxy(x + width + 1, y + height + 1);
    cout << CORNER_RIGHT_BOT;
}
void deleteContent(int x, int y, int width, int height)
{
    for (int i = x; i <x + width + 1; i++)
    {
        for (int j = y; j < y + height + 1; j++)
        {
            gotoxy(i , j );
            cout << " ";
        }
    }
}
void ShowCur(bool CursorVisibility)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
    SetConsoleCursorInfo(handle, &cursor);
}
void displayPercent(int &number_of_file, vector<File> files, int &width_max)
{
    if (number_of_file != files.size())
    {
        deleteContent(40, 10, width_max + 26, files.size());

        for (int i = 0; i < files.size(); i++)
        {

            int len = strlen(files[i].file_name);
            if (width_max < len) width_max = len;
        }


        createBox(40, 10, width_max + 24, files.size());
        for (int i = 0; i < files.size(); i++)
        {
            gotoxy(40 + 1, 11 + i);
            cout << "Downloading " << files[i].file_name << " ...";
        }
        
        number_of_file = files.size();

    }
}
//======================= End Display=============================================
void ReceiveInfoAllFileFromServer(CSocket& client)
{
    cout << "LIST FILE\n";
    string file_name = "file.txt";
    ofstream fout;
    fout.open(file_name, ios::out | ios::binary);
    int byte;
    client.Receive((char*)&byte, sizeof(byte), 0);
    char* msg = new char[byte + 1];
    client.Receive(msg, byte, 0);
    msg[byte] = '\0';

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
    cout << "\n";
}
void Receive1Chunk(CSocket& client, vector<pair<ofstream, File>>& v, int index)
{

    if (v[index].second.current_size_file + size_buff <= v[index].second.size_file)
    {
        int total_byte_sum = 0;
        char* buff_receive = new char[size_buff];
        int byte = client.Receive(buff_receive, size_buff, 0);
        v[index].first.write(buff_receive, byte);
        total_byte_sum += byte;
        client.Send((char*)&total_byte_sum, sizeof(total_byte_sum), 0);
        while (total_byte_sum < size_buff)
        {
            byte = client.Receive(buff_receive, size_buff - total_byte_sum, 0);
            v[index].first.write(buff_receive, byte);
            total_byte_sum += byte;
            client.Send((char*)&total_byte_sum, sizeof(total_byte_sum), 0);
        }
        v[index].second.current_size_file += size_buff;
        delete[]buff_receive;
    }
    else
    {
        int total_byte_sum = 0;
        int byte_send = v[index].second.size_file - v[index].second.current_size_file;
        char* buff_receive = new char[byte_send];
        int byte = client.Receive(buff_receive, byte_send, 0);
        v[index].first.write(buff_receive, byte);

        total_byte_sum += byte;
        client.Send((char*)&total_byte_sum, sizeof(total_byte_sum), 0);
        while (total_byte_sum < byte_send)
        {
            byte = client.Receive(buff_receive, byte_send - total_byte_sum, 0);
            v[index].first.write(buff_receive, byte);
            total_byte_sum += byte;
            client.Send((char*)&total_byte_sum, sizeof(total_byte_sum), 0);
        }
        v[index].second.current_size_file += byte_send;
        delete[]buff_receive;
    }
   
}
void checkIsUpdate(CSocket& client, vector<pair<ofstream, File>>& v, vector<File>& tmp)
{
    int num_of_file = 0;
    int size_cur = tmp.size();
    if (size_cur - size_pre > 0)
    {
        num_of_file = size_cur - size_pre;
        size_pre = size_cur;
    }
    client.Send(&num_of_file, sizeof(num_of_file), 0);
    if (num_of_file > 0)
    {
        int v_size = size_pre;
        for (int i = size_cur - num_of_file; i < size_cur; i++)
        {
            tmp[i].size_file_name = strlen(tmp[i].file_name);
            client.Send((char*)&tmp[i], sizeof(tmp[i]), 0);
            int byte_file_name = strlen(tmp[i].file_name);
            client.Send((char*)&byte_file_name, sizeof(byte_file_name), 0);
            client.Send(tmp[i].file_name, byte_file_name, 0);
            client.Receive((char*)&(tmp[i].size_file), sizeof(tmp[i].size_file), 0);
            string file_name = tmp[i].file_name;

            ofstream fout(("output/" + file_name).c_str(), ios::binary | ios::trunc);
            if (!fout)
            {
                cout << "Can not open file\n";
                return;
            }
            File temp;
            temp.current_size_file = tmp[i].current_size_file;

            int len = strlen(tmp[i].file_name);
            temp.file_name = new char[len + 1];
            strcpy_s(temp.file_name,len + 1, tmp[i].file_name);
            temp.file_name[len] = '\0';

            temp.priority = tmp[i].priority;
            temp.size_file = tmp[i].size_file;
            temp.size_file_name = tmp[i].size_file_name;
           
            v.push_back({ move(fout), temp });
        }
    }
}
void ReceiveFileDownloadToClient(CSocket& client, vector<pair<ofstream, File>>& v, vector<File>& tmp, int width_max)
{
    {
        checkIsUpdate(ref(client), v, tmp);
        std::lock_guard<std::mutex> guard(mtx);
    }
    for (int i = 0; i < tmp.size(); i++) //edit
    {
        if (tmp[i].current_size_file == tmp[i].size_file)
        {
            gotoxy(40 + width_max + 20, 11 + i);
            cout << "100%";
        }
    }
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].second.priority; j++)
        {
            //Todo: receive 1 chunk 1024 byte to client
            Receive1Chunk(client, v, i);
            // ============= Display DownLoad ========================
            for (int k = 0; k < tmp.size(); k++) //edit
            {
                gotoxy(40 + width_max + 20, 11 + k);
                if (strcmp(v[i].second.file_name, tmp[k].file_name) == 0 )
                {
                    cout << (v[i].second.current_size_file) * 100 / v[i].second.size_file << "%" << endl;
                    break;
                }  
            }
            // ============= Display DownLoad ========================

            if (v[i].second.current_size_file == v[i].second.size_file)
            {
                for (int k = 0; k < tmp.size(); k++)
                {
                    if (strcmp(v[i].second.file_name, tmp[k].file_name) == 0)
                    {
                        tmp[k].current_size_file = tmp[k].size_file;
                        break;
                    }
                }
                v[i].first.close();
                delete[] v[i].second.file_name;
                v[i].second.file_name = NULL;         
                v.erase(v.begin() + i);
                i--;
                break;
            }
        }
    }
    //Check is modify.
}


//Thread
vector<string> readListFile()
{
    ifstream fin;
    vector<string> list_file;

    fin.open("file.txt");
    if (!fin.is_open())
    {
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
void checkInput(vector<File>& files, vector<string>list_file)
{
    ifstream fin;
    vector<string> file_need;
    int num_of_line = 0;

    while (true)
    {
        fin.open("input.txt");
        if (!fin.is_open())
        {
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
                continue;
            }
            bool is_exist = false;
            for (int i = 0; i < file_need.size(); i++)
            {
                if (temp_2 == file_need[i])
                {
                    is_exist = true;
                    break;
                }
            }
            if (is_exist)continue;
            string temp_3;
            getline(ss, temp_3, '\n');
            if (temp_3 != "NORMAL" && temp_3 != "HIGH" && temp_3 != "CRITICAL")
            {
                continue;
            }
          
            File file_new;

            file_new.file_name = new char[temp_2.length() + 1];
            strcpy_s(file_new.file_name, temp_2.length() + 1, temp_2.c_str());
            file_new.file_name[temp_2.length()] = '\0';

            if (temp_3 == "NORMAL") file_new.priority = 1;
            else if (temp_3 == "HIGH") file_new.priority = 4;
            else if (temp_3 == "CRITICAL") file_new.priority = 10;
            file_need.push_back(file_new.file_name);
            {
                std::lock_guard<std::mutex> guard(mtx);
                files.push_back(file_new);
            }
        }
        fin.close();
        this_thread::sleep_for(std::chrono::seconds(2));
    }
}
//======
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
