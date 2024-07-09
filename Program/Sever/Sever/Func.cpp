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
void ReadFileNeedDownFromClient(CSocket& client)
{
    unsigned long long len = 0;
    client.Receive((char*)&len, sizeof(len), 0);//Receives length of string
    char* buffer_receive = new char[len + 1];
    client.Receive(buffer_receive, len, 0);//Receives string
    buffer_receive[len] = '\0';

    string file_recei = "list_file_recei.txt";

    writeFileRecei(file_recei, buffer_receive);

    delete[] buffer_receive;
}
void SendInfo1FileToClient(CSocket& client, string temp_2)
{
    cout << "Sending file " << temp_2 << " for client!\n";
    unsigned long long size_file = readSizeFile(temp_2);
    unsigned long long size_tmp2 = temp_2.length();
    client.Send((char*)&size_tmp2, sizeof(size_tmp2), 0);//Send length of name file

    char* name_file = new char[size_tmp2 + 1];
    strcpy_s(name_file, size_tmp2 + 1, temp_2.c_str());
    name_file[size_tmp2] = '\0';

    client.Send(name_file, size_tmp2, 0);//Send name file
    client.Send(&size_file, sizeof(size_file), 0);//Send size of file
}
void SendInfoAllFileToClient(CSocket& client)
{
    string list_file = "list_file.txt";
    cout << "Sending list file!\n";
    unsigned long long size_list_file = readSizeFile(list_file);
    client.Send((char*)&size_list_file, sizeof(size_list_file), 0);
    ifstream in;
    in.open(list_file, ios::binary);
    if (!in.is_open())
    {
        cout << "Send list file falled!\n";
        return;
    }
    else
    {
        char* buffer = new char[size_list_file];
        in.read(buffer, size_list_file);
        client.Send(buffer, size_list_file, 0);
        delete[] buffer;
        cout << "Sending file text is successful!\n";
        in.close();
    }
}
void Send1FileToClient(CSocket& client, string fileNeedDown)
{
    ifstream in_send;
    in_send.open(fileNeedDown.c_str(), ios::binary);
    int size_buff = 1024;
    char* buff_send = new char[size_buff];
    unsigned long long total_byte = readSizeFile(fileNeedDown);
    unsigned long long byte_sum = 0;
    while (byte_sum < total_byte)
    {
        if (byte_sum + size_buff <= total_byte)
        {
            in_send.read(buff_send, size_buff);
            client.Send(buff_send, size_buff, 0);
            byte_sum = byte_sum + size_buff;
        }
        else
        {
            in_send.read(buff_send, total_byte - byte_sum);
            client.Send(buff_send, total_byte - byte_sum, 0);
            byte_sum = byte_sum + size_buff;
        }
    }
    delete[]buff_send;
    in_send.close();
    cout << "Client download file successful!\n";
}
