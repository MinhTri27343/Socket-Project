#include"Func.h"
#include "pch.h"
#include"framework.h"
using namespace std;

std::mutex mtx;

void SignalCallBack(int signum) {
    exit(signum);
}
void ReceiveInfoAllFileFromServer(CSocket& client)
{
    string file_name = "file.txt";
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
void gotoxy(unsigned long long x, unsigned long long y)
{
    static HANDLE h = NULL;
    if (!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { x, y };
    SetConsoleCursorPosition(h, c);
}
void Receive1Chunk(CSocket& client, vector<pair<ofstream, File>>& v, int index)
{
    if (v[index].second.current_size_file + size_buff <= v[index].second.size_file)
    {
        char* buff_receive = new char[size_buff];
        client.Receive(buff_receive, size_buff, 0);
        v[index].first.write(buff_receive, size_buff);
        v[index].second.current_size_file += size_buff;
    }
    else
    {
        int byte_send = v[index].second.size_file - v[index].second.current_size_file;
        char* buff_receive = new char[byte_send];
        client.Receive(buff_receive, byte_send, 0);
        v[index].first.write(buff_receive, byte_send);
        v[index].second.current_size_file += byte_send;
    }
}
void checkIsUpdate(CSocket& client, vector<pair<ofstream, File>>& v, vector<File>& tmp)
{
    bool isChange = false;
    if (tmp.size() > 0)
    {
        isChange = true;
        client.Send((char*)&isChange, sizeof(isChange), 0);
        int num_of_file = tmp.size();
        client.Send((char*)&num_of_file, sizeof(num_of_file), 0);
        for (int i = 0; i < num_of_file; i++)
        {
            tmp[i].size_file_name = strlen(tmp[i].file_name);
            client.Send((char*)&tmp[i], sizeof(tmp[i]), 0);
            int byte_file_name = strlen(tmp[i].file_name);
            client.Send((char*)&byte_file_name, sizeof(byte_file_name), 0);
            client.Send(tmp[i].file_name, byte_file_name, 0);
            client.Receive((char*)&(tmp[i].size_file), sizeof(tmp[i].size_file), 0);
            ofstream fout(tmp[i].file_name, ios::binary | ios::trunc);
            v.push_back({ move(fout), tmp[i] });
            tmp.erase(tmp.begin());
            num_of_file--;
            i--;
        }
    }
    else
    {
        client.Send((char*)&isChange, sizeof(isChange), 0);
    }
}
void ReceiveFileDownloadToClient(CSocket& client, vector<pair<ofstream, File>>& v, vector<File>& tmp)
{
    {
        std::lock_guard<std::mutex>lock(mtx);
        checkIsUpdate(ref(client), v, tmp);
    }
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].second.priority; j++)
        {
            //Todo: receive 1 chunk 1024 byte to client
            Receive1Chunk(client, v, i);
            gotoxy(0, 25 + i);
            cout << (v[i].second.current_size_file) * 100 / v[i].second.size_file << "%" << endl;
            if (v[i].second.current_size_file == v[i].second.size_file)
            {
                gotoxy(0, 25 + i);
                cout << 100 << "%";
                v[i].first.close();
                v.erase(v.begin() + i);
                i--;
                break;
            }
        }
        bool isDone = true;
        client.Send((char*)&isDone, sizeof(isDone), 0);
    }
    //Check is modify.
}
vector<string> readListFile()
{
    ifstream fin;
    vector<string> list_file;

    fin.open("file.txt");
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
            else if (temp_2 == "HIGH") file_new.priority = 4;
            else if (temp_2 == "CRITICAL") file_new.priority = 10;
            file_need.push_back(file_new.file_name);
            {
                std::lock_guard<std::mutex>lock(mtx);
                files.push_back(file_new);
            }
        }
        fin.close();
        this_thread::sleep_for(std::chrono::seconds(2));
    }
}
