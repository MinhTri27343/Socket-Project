#include"Func.h"
#include"pch.h"

void writeFileRecei(string file_recei, char* buffer_receive)
{
    ofstream out;
    stringstream ss(buffer_receive);

    out.open(file_recei, ios::trunc);//Overwrite
    if (!out.is_open())
    {
        cout << "Write file error";
    }
    else
    {
        string temp_1;
        while (ss >> temp_1)
        {
            out << temp_1 << "\n";
        }
        out.close();
        //cout << "Received file need to download!\n";
    }


}
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
bool ReadFileNeedDownFromClient(CSocket& client)
{
    int len = 0;
    if (client.Receive((char*)&len, sizeof(len), 0) == SOCKET_ERROR)//Receives length of string
        return false;
    char* buffer_receive = new char[len + 1];
    if (client.Receive(buffer_receive, len, 0) == SOCKET_ERROR)//Receives string
        return false;
    buffer_receive[len] = '\0';

    string file_recei = "list_file_recei.txt";

    writeFileRecei(file_recei, buffer_receive);

    delete[] buffer_receive;
    return true;
}
bool SendInfo1FileToClient(CSocket& client, string temp_2)
{
    cout << "Sending file " << temp_2 << " for client!\n";
    unsigned long long size_file = readSizeFile(temp_2);
    int size_tmp2 = temp_2.length();
    if (client.Send((char*)&size_tmp2, sizeof(size_tmp2), 0) == SOCKET_ERROR)//Send length of name file
        return false;
    char* name_file = new char[size_tmp2 + 1];
    strcpy_s(name_file, size_tmp2 + 1, temp_2.c_str());
    name_file[size_tmp2] = '\0';

    if (client.Send(name_file, size_tmp2, 0) == SOCKET_ERROR)//Send name file
        return false;
    if (client.Send(&size_file, sizeof(size_file), 0) == SOCKET_ERROR)
        return false;//Send size of file
    return true;
}
bool SendInfoAllFileToClient(CSocket& client)
{
    string list_file = "list_file.txt";
    cout << "Sending list file to client!\n";
    int size_list_file = readSizeFile(list_file);
    if (client.Send((char*)&size_list_file, sizeof(size_list_file), 0) == SOCKET_ERROR)
        return false;
    ifstream in;
    in.open(list_file, ios::binary);
    if (!in.is_open())
    {
        cout << "Sending list file falled!\n";
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
bool Send1FileToClient(CSocket& client, string fileNeedDown)
{
    ifstream in_send;
    in_send.open(fileNeedDown.c_str(), ios::binary);
    int size_buff = 32768;
    unsigned long long byte_sum = 0;

    while (!in_send.eof())
    {
        char* buff_send = new char[size_buff];
        in_send.read(buff_send, size_buff);
        int byte_read = in_send.gcount();

        int byte_send = client.Send(buff_send, byte_read, 0);
        if (byte_send == SOCKET_ERROR) return false;

        while (byte_send < byte_read)
        {
            int temp_byte_send;
            temp_byte_send = client.Send(buff_send + byte_send, byte_read - byte_send, 0);
            if (temp_byte_send == SOCKET_ERROR) return false;
            byte_send = temp_byte_send + byte_send;
        }

        byte_sum += byte_send;
        delete[]buff_send;

    }
    bool isDone = false;
    client.Receive((char*)&isDone, sizeof(isDone));
    in_send.close();
    cout << "Client download file"<<fileNeedDown<<" successful!\n";
    cout << "\n";
    return true;
}
