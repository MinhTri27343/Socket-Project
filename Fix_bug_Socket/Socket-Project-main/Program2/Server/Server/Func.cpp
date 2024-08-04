#include"Func.h"
#include "pch.h"
#include "framework.h"
using namespace std;

unsigned long long readSizeFile(string file_name)
{
    ifstream in;
    in.open(file_name, ios::binary);
    unsigned long long size = 0;
    if (!in.is_open())
    {
        cout << "Can not open file\n";
        return size;
    }
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
void isDone(CSocket& client)
{
    bool isDone;
    client.Receive((char*)&isDone, sizeof(isDone), 0);
}
bool checkError(char* source, char* destination, int size)
{
    return memcmp(source, destination, size) != 0;
}
void Send1Chunk(CSocket& client, vector<pair<ifstream, File>>& v, int index)
{
    if (v[index].second.current_size_file + size_buff <= v[index].second.size_file)
    {
        v[index].first.seekg(v[index].second.current_size_file, 0); //edit
        char* buff_send = new char[size_buff];
        v[index].first.read(buff_send, size_buff);
        //for (int i = 0; i < 100; i++)
            //cout << buff_send[i];
        //cout << endl;
        //isDone(ref(client));
        int byte = client.Send(buff_send, size_buff, 0);

        //isDone(ref(client));

        //this_thread::sleep_for(std::chrono::milliseconds(500));
        char* destination = new char[size_buff];
        client.Receive(destination, size_buff, 0);

        //isDone(ref(client));

        //this_thread::sleep_for(std::chrono::milliseconds(500));
        bool check = true; //check co error khong
        while (checkError(buff_send, destination, size_buff) == true)
        {
            client.Send((char*)&check, sizeof(check), 0);

            //v[index].first.seekg(v[index].second.current_size_file, 0); //edit
            //v[index].first.read(buff_send, size_buff);
            client.Send(buff_send, size_buff, 0);

            //isDone(ref(client));

            //this_thread::sleep_for(std::chrono::milliseconds(500));
            delete[]destination; destination = new char[size_buff];
            client.Receive(destination, size_buff, 0);

            //isDone(ref(client));
            //this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        check = false;
        client.Send((char*)&check, sizeof(check), 0);
        v[index].second.current_size_file += size_buff;
    }
    else
    {
        v[index].first.seekg(v[index].second.current_size_file, 0); //edit
        int byte_send = v[index].second.size_file - v[index].second.current_size_file;
        char* buff_send = new char[byte_send];
        v[index].first.read(buff_send, byte_send);
        //isDone(ref(client));
        int byte = client.Send(buff_send, byte_send, 0);
        //isDone(ref(client));
        //this_thread::sleep_for(std::chrono::milliseconds(500));

        char* destination = new char[byte_send];
        client.Receive(destination, byte_send, 0);

        //isDone(ref(client));
        //this_thread::sleep_for(std::chrono::milliseconds(500));
        bool check = true; //check co error khong
        while (checkError(buff_send, destination, byte_send) == true)
        {
            client.Send((char*)&check, sizeof(check), 0);

            //v[index].first.seekg(v[index].second.current_size_file, 0); //edit
            //v[index].first.read(buff_send, byte_send);
            client.Send(buff_send, byte_send, 0);
            //isDone(ref(client));
            //this_thread::sleep_for(std::chrono::milliseconds(500));
            delete[]destination; destination = new char[byte_send];
            client.Receive(destination, byte_send, 0);
            //this_thread::sleep_for(std::chrono::milliseconds(500));
            //isDone(ref(client));
        }
        check = false;
        client.Send((char*)&check, sizeof(check), 0);

        v[index].second.current_size_file += byte_send;
    }
}
void isCheckUpdate(CSocket& client, vector<pair<ifstream, File>>& v)
{
    //Check is modify.
    int num_of_file;
    //bool isSuccessfull = true;
    client.Receive((char*)&num_of_file, sizeof(num_of_file), 0);
    //client.Send(&isSuccessfull, sizeof(bool), 0);
    if (num_of_file > 0)
    {
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
void SendFileDownloadToClient(CSocket& client, vector<pair<ifstream, File>>& v)
{
    //isCheckUpdate(ref(client), v);
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].second.priority; j++)
        {
            //Todo: send 1 chunk 1024 byte to client
            Send1Chunk(ref(client), v, i); //if don't have enough 1 chunk equal to 1024 byte, send all
            //want to delete 1 file in vector to do here
            //this_thread::sleep_for(std::chrono::milliseconds(2000));
            if (v[i].second.current_size_file == v[i].second.size_file)
            {
                cout << "Hello" << endl;
                v[i].first.close();
                delete[] v[i].second.file_name;
                v[i].second.file_name = NULL;
                v.erase(v.begin() + i);
                i--;
                break;
            }
        }
        //bool isDone;
        //client.Receive((char*)&isDone, sizeof(isDone), 0);
    }
}
