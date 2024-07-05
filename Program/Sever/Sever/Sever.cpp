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
                string file_name = "list_file.txt";
                cout << "Sendding file!\n";
                int byte_read = 1;

                ifstream in;

                in.open(file_name,ios::binary);//open file
                if (!in.is_open())
                {
                    cout << "Reading file error";
                }
                else
                {
                    char* buffer = new char[byte_read];
                    while (in.read(buffer, byte_read))
                    {
                        client.Send(buffer, byte_read, 0);
                    }

                    in.close();
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
                    //===========Split string and write in list file need download=======================
                    ofstream out;
                    stringstream ss(buffer_receive);
                    string file_recei="list_file_recei.txt";
                    out.open(file_recei,ios::trunc);//Overwrite
                    if (!out.is_open())
                    {
                        cout << "Write file error";
                    }
                    else
                    {
                        string temp_1;
                        while (ss >> temp_1)
                        {
                            out <<temp_1<<"\n";
                        }
                        out.close();
                    }

                    //===========Split string and write in list file need download========================
                    //===========Send file ==============================================================
                    
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
                            if (temp_2 == "")continue;cout<<""
                            //=======================Send file temp_2======================================
                            




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
