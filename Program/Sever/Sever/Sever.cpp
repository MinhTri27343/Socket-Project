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
            cout << "Server is listening!\n";
            sever.Listen();
            if (sever.Accept(client))
            {
                cout << "Connect is succesful!\n";

                //===========================================Sent file list_file=============================================================
                SendInfoAllFileToClient(ref(client));
                //===========================================Sent file list_file=============================================================

                //============================================Receive list file need download=====================================================
                while (true)
                {
                    ifstream in;
                    ReadFileNeedDownFromClient(ref(client));
                    cout << "Received file need to download!\n";
                    //===========Send file ==============================================================
                    string file_recei = "list_file_recei.txt";
                    in.open(file_recei);

                    if (!in.is_open())
                    {
                        cout << "Read file error";
                        return 0;
                    }
                    else
                    {
                        string temp_2;
                        while (getline(in, temp_2, '\n'))
                        {
                            SendInfo1FileToClient(ref(client), temp_2); //đúng rồi khỏi sửa
                            Send1FileToClient(ref(client), temp_2);
                        }
                        in.close();
                        //===========Send file ============================================================
                    }
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
