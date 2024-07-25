//client
#define NORMAL 1
#define HIGH 4
#define CRITICAL 10
#define size_buff 20480
struct File
{
    int size_file_name = 0;
    char* file_name = NULL;
    unsigned long long size_file = 0;
    unsigned long long current_size_file = 0;
    int priority = 0;
};
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
    checkIsUpdate(ref(client), v, tmp);
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].second.priority; j++)
        {
            //Todo: receive 1 chunk 1024 byte to client
            Receive1Chunk(client, v, i);
            //gotoxy(0, 10 + i);
            //cout << (v[i].second.current_size_file) * 100 / v[i].second.size_file << "%" << endl;
            if (v[i].second.current_size_file == v[i].second.size_file)
            {
                //gotoxy(0, 10 + i);
                //cout << 100 << "%";
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

//Server
#define NORMAL 1
#define HIGH 4
#define CRITICAL 10
#define size_buff 20480
struct File
{
    int size_file_name = 0;
    char* file_name = NULL;
    unsigned long long size_file = 0;
    unsigned long long current_size_file = 0;
    int priority = 0;
};
unsigned long long readSizeFile(string file_name)
{
    ifstream in;
    in.open(file_name, ios::binary);
    unsigned long long size = 0;
    if (!in.is_open())return size;
    else
    {
        in.seekg(0, ios::end);
        size = in.tellg();

        in.close();
    }
    return size;
}
bool SendInfoAllFileToClient(CSocket& client)
{
    string list_file = "list_file.txt";
    cout << "Sending list file!\n";
    unsigned long long size_list_file = readSizeFile(list_file);
    if (client.Send((char*)&size_list_file, sizeof(size_list_file), 0) == SOCKET_ERROR)
        return false;
    ifstream in;
    in.open(list_file, ios::binary);
    if (!in.is_open())
    {
        cout << "Send list file falled!\n";
        return true;
    }
    else
    {
        char* buffer = new char[size_list_file];
        in.read(buffer, size_list_file);
        if (client.Send(buffer, size_list_file, 0) == SOCKET_ERROR)
            return false;
        delete[] buffer;
        cout << "Sending file text is successful!\n";
        in.close();
    }
    return true;
}
void Send1Chunk(CSocket& client, vector<pair<ifstream, File>>& v, int index)
{
    if (v[index].second.current_size_file + size_buff <= v[index].second.size_file)
    {
        char* buff_send = new char[size_buff];
        v[index].first.read(buff_send, size_buff);
        client.Send(buff_send, size_buff, 0);
        v[index].second.current_size_file += size_buff;
    }
    else
    {
        int byte_send = v[index].second.size_file - v[index].second.current_size_file;
        char* buff_send = new char[byte_send];
        v[index].first.read(buff_send, byte_send);
        client.Send(buff_send, byte_send, 0);
        v[index].second.current_size_file += byte_send;
    }
}
void isCheckUpdate(CSocket& client, vector<pair<ifstream, File>>& v)
{
    //Check is modify.
    bool isChange;
    client.Receive((char*)&isChange, sizeof(isChange), 0);
    if (isChange == true)
    {
        int num_of_file;
        client.Receive((char*)&num_of_file, sizeof(num_of_file), 0);
        for (int i = 0; i < num_of_file; i++)
        {
            File tmp;
            client.Receive((char*)&tmp, sizeof(tmp), 0);
            int byte_file_name;
            client.Receive((char*)&byte_file_name, sizeof(byte_file_name), 0);
            tmp.file_name = new char[byte_file_name + 1];
            client.Receive(tmp.file_name, byte_file_name, 0);
            tmp.file_name[byte_file_name] = '\0';
            tmp.size_file = readSizeFile(tmp.file_name);
            client.Send((char*)&(tmp.size_file), sizeof(tmp.size_file), 0);
            ifstream fin(tmp.file_name, ios::binary);
            v.push_back({ move(fin), tmp });
        }
    }
}
void SendFileDownloadToClient(CSocket& client, vector<pair<ifstream, File>>&v)
{
    isCheckUpdate(ref(client), v);
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].second.priority; j++)
        {
            //Todo: send 1 chunk 1024 byte to client
            Send1Chunk(ref(client), v, i); //if don't have enough 1 chunk equal to 1024 byte, send all
            //want to delete 1 file in vector to do here
            if (v[i].second.current_size_file == v[i].second.size_file)
            {
                v[i].first.close();
                v.erase(v.begin() + i);
                i--;
                break;
            }
        }   
        bool isDone;
        client.Receive((char*)&isDone, sizeof(isDone), 0);
    }
}
