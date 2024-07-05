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
            cout << "Sever is listenning!\n";
            sever.Listen();
            if (sever.Accept(client))
            {
                cout << "COnnect is succesfull!\n";

                //===========================================Sent file list_file=============================================================
                string file_name = "E:\\HK3-2024\\MMT\\Project_Socket\\Project\\list_file.txt";
                cout << "Sendding file\n";
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
                    while (!in.eof())
                    {
                        in.read(buffer, byte_read);
                        client.Send(buffer, byte_read, 0);
                    }

                    in.close();
                    delete[] buffer;
                }
                //===========================================Sent file list_file=============================================================


               



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
