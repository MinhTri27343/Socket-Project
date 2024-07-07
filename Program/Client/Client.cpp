// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "Func.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: code your application's behavior here.
            //Initialize
            AfxSocketInit(NULL);
            CSocket client;
            char ip[100];
            int port;
            client.Create();

            // input
            cout << "Input IP address of server: "; cin >> ip;
            cout << "Input port of server: "; cin >> port;

            if (client.Connect(CA2W(ip), port))
            {

// =============================================== Duc Tri ====================================================================
                cout << "\nClient has connected to the server";
                cout << endl;
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
                signal(SIGINT, SignalCallBack);
 // ============================================== Duc Tri ====================================================================


// =============================================== Minh tri ====================================================================


                // ================= Sending file name need to download for server ======================
                string file_user = "input.txt";

                unsigned long long size_pre_file = getByteSum(file_user);
                unsigned long long size_after_file = getByteSum(file_user);
                string file_check1 = file_user;
                string file_check2 = file_name;
                while (true)
                {
                    ifstream in(file_user);
                    if (!in)
                    {
                        cout << "\nCan not open file \n";
                        return 0;
                    }
                    size_after_file = getByteSum(file_user);
                    if (size_after_file > size_pre_file)
                    {
                        unsigned long long byte_size = size_after_file - size_pre_file;
                        in.seekg(size_pre_file, ios::beg);
                        char* information_file = new char[byte_size + 1];
                        in.read(information_file, byte_size);
                        information_file[byte_size] = '\0';
                        char* temp = normalizeChar(information_file);
                        char* send = fileDownload(temp, file_check1, file_check2, size_pre_file);
                        unsigned long long size_send = strlen(send);
                        if (size_send > 0)
                        {
                            client.Send((char*)&size_send, sizeof(size_send), 0);
                            client.Send(send, size_send, 0);
                        }
                        delete[] information_file;
                        delete[] temp;
                        delete[] send;
                        size_pre_file = size_after_file;

                        // ================= Sending file name need to download for server ======================

                        // ===================== Ghi nhan ten file download tu server  =============================

                        // Initialize
                        unsigned long long size_name_file_download = 0;
                        unsigned long long size_file_download = 0;
                        char* name_file_download = NULL;
                        char* file_download = NULL;
                        unsigned long long byte_read = 1;

                        // Ghi nhan ten file download 
                        client.Receive((char*)&size_name_file_download, sizeof(size_name_file_download), 0);

                        name_file_download = new char[size_name_file_download + 1];
                        client.Receive(name_file_download, size_name_file_download, 0);
                        name_file_download[size_name_file_download] = '\0';
                        string name_file_download_str = name_file_download;


                        // Ghi nhan byte cua file download 
                        client.Receive((char*)&size_file_download, sizeof(size_file_download), 0);

                        // ===================== Ghi nhan ten  file download tu server  =============================

                        // ================== Display console percent of file downloading ================

                        // Initialize
                        ShowCur(0);
                        unsigned long long width = 26 + name_file_download_str.size();
                        unsigned long long height = 2;
                        unsigned long long byte_sum = getByteSum(name_file_download_str);
                        unsigned long long coordinate_x = 30;
                        unsigned long long coordinate_y = 4;

                        createBox(coordinate_x, coordinate_y, width, height);
                        gotoxy(coordinate_x + 1, coordinate_y + 1);
                        cout << "Downloading " << name_file_download_str << " .... ";


                        ofstream out(name_file_download_str); // test truoc roi them vao folder output sau

                        unsigned long long total_byte_curr = 0;
                        while (total_byte_curr < size_file_download)
                        {
                            // receive byte of file download from server  
                            char* read_byte_file_download = new char[byte_read];
                            if (size_file_download - total_byte_curr < byte_read)
                            {
                                client.Receive(read_byte_file_download, size_file_download - total_byte_curr);
                                out.write(read_byte_file_download, size_file_download - total_byte_curr);
                                total_byte_curr = size_file_download;

                            }
                            else
                            {
                                total_byte_curr += byte_read;
                                client.Receive(read_byte_file_download, total_byte_curr);
                                out.write(read_byte_file_download, byte_read);
                            }
                            gotoxy(coordinate_x + 20 + name_file_download_str.size(), coordinate_y + 1);
                            cout << (total_byte_curr * 100) / byte_sum << "%";
                            Sleep(100); // Sleep for see downloading 
                            delete[] read_byte_file_download;
                        }


                        delete[] name_file_download;
                        delete[] file_download;
                        out.close();


                    }
                    // ================== Display console percent of file downloading ================
                    in.close();
                }

              
            }

// =============================================== Minh tri ====================================================================
            else
            {
                cout << "\n Unable to connect to the server";
            }

            client.Close();

        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
