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
    int size_list_file = readSizeFile(list_file);
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
        cout << "Sending list file is successful!\n";
        cout << "\n";
        in.close();
    }
    return true;
}
bool Send1Chunk(CSocket& client, vector<pair<ifstream, File>>& v, int index)
{
    
    if (v[index].second.current_size_file + size_buff <= v[index].second.size_file)
    {
        
        char* buff_send = new char[size_buff];
        v[index].first.read(buff_send, size_buff);
        if (client.Send(buff_send, size_buff, 0) == SOCKET_ERROR)
            return false;
        v[index].second.current_size_file += size_buff;
        delete[]buff_send;
    }
    else
    {
     
        
        int byte_send = v[index].second.size_file - v[index].second.current_size_file;
        char* buff_send = new char[byte_send];
        v[index].first.read(buff_send, byte_send);
        if (client.Send(buff_send, byte_send, 0) == SOCKET_ERROR)
            return false;
      
        v[index].second.current_size_file += byte_send;
        delete[]buff_send;
    }
    return true;
}
bool isCheckUpdate(CSocket& client, vector<pair<ifstream, File>>& v)
{
    //Check is modify.
    int num_of_file;
    if (client.Receive((char*)&num_of_file, sizeof(num_of_file), 0) == SOCKET_ERROR)
        return false;
    if (num_of_file > 0)
    {
        for (int i = 0; i < num_of_file; i++)
        {
            File tmp;
            if (client.Receive((char*)&tmp, sizeof(tmp), 0) == SOCKET_ERROR)
                return false;
            int byte_file_name;
            if (client.Receive((char*)&byte_file_name, sizeof(byte_file_name), 0) == SOCKET_ERROR)
                return false;
            tmp.file_name = new char[byte_file_name + 1];
            if (client.Receive(tmp.file_name, byte_file_name, 0) == SOCKET_ERROR)
                return false;
            tmp.file_name[byte_file_name] = '\0';
            tmp.size_file = readSizeFile(tmp.file_name);
            if (client.Send((char*)&(tmp.size_file), sizeof(tmp.size_file), 0) == SOCKET_ERROR)
                return false;
            ifstream fin(tmp.file_name, ios::binary);
            v.push_back({ move(fin), tmp });
        }
    }
    return true;
}
bool SendFileDownloadToClient(CSocket& client, vector<pair<ifstream, File>>& v)
{
    if (isCheckUpdate(ref(client), v) == false)
        return false;
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].second.priority; j++)
        {
           

            if (Send1Chunk(ref(client), v, i) == false)
                return false;
            if (v[i].second.current_size_file == v[i].second.size_file)
            {
                cout << "Sent file " << v[i].second.file_name << " to client.\n";

                v[i].first.close();
                delete[] v[i].second.file_name;
                v[i].second.file_name = NULL;
                v.erase(v.begin() + i);
                i--;
                break;
            }
        }
    }
    return true;
}
