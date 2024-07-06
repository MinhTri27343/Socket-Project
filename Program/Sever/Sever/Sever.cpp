// Sever.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Sever.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;


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
            // =============================================================




            AfxSocketInit(NULL);
            CSocket sever;
            CSocket client;
            sever.Create(1234);
            cout << "Server is listenning!\n";
            sever.Listen();
            if (sever.Accept(client))
            {
                cout << "Connect is succesfull!\n";

                //===========================================Sent file list_file=============================================================
                string list_file = "list_file.txt";
                cout << "Sendding file!\n";
                unsigned long long size_list_file = readSizeFile(list_file);
                cout << size_list_file;
                client.Send(&size_list_file, sizeof(size_list_file), 0);
                ifstream in;
                in.open(list_file,ios::binary);  
                if(!in.is_open())
                {

                    cout << "Send list file falled!\n";
                }
                else
                {
                    char* buffer = new char[size_list_file];
                    in.read(buffer, size_list_file);
                    client.Send(buffer, size_list_file, 0);
                    delete[] buffer;
                    cout << "Sending file text is successful!\n";
                }
                //===========================================Sent file list_file=============================================================

                //============================================Receive list file need download=====================================================
                 
                while (true)
                {
                    int len = 0;
                    client.Receive(&len, sizeof(len), 0);//Receives length of string
                    char* buffer_receive = new char[len + 1];
                    client.Receive(buffer_receive, len, 0);//Receives string
                    buffer_receive[len] = '\0';
                  
                    //===========Send file ==============================================================
                    string file_recei = "list_file_recei.txt";

                    writeFileRecei(file_recei, buffer_receive);
                    
                    in.open(file_recei);
                    if (!in.is_open())
                    {
                        cout << "Read file error";
                    }
                    else
                    {
                        string temp_2;
                        while (!in.eof())
                        {
                            getline(in, temp_2, '\n');
                            if (temp_2 == "")continue; 
                            //=======================Send file temp_2======================================
                            unsigned long long size_file = readSizeFile(temp_2);

                            client.Send(&size_file, sizeof(size_file), 0);

                            ifstream in_send;
                            in_send.open(temp_2, ios::binary);
                            int size_buff = 1;
                            char* buff_send = new char[size_buff];
                            while (in_send.read(buff_send, size_buff))
                            {
                                client.Send(buff_send, size_buff, 0);
                            }
                            //=======================Send file temp_2======================================



                        }
                    }

                  
                    //===========Send file =============================================================

                }


                
                //============================================Receive list file need download=====================================================

               



            }
            else
            {
                cout << "Connect is falled!\n";
                
            }


            // =============================================================

        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }
    system("pause");
    return nRetCode;
}
