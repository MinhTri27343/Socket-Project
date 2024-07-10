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
            sever.Create(1234);
            cout << "Server is listening!\n";
            sever.Listen();
            int n;
            cout << "Input the number of clients: ";
            cin >> n;
            CSocket* client = new CSocket[n];
            for (int i = 0; i < n; i++)
            {
                if (sever.Accept(client[i]))
                {
                    cout << "Connect is succesful!\n";

                    //===========================================Sent file list_file=============================================================
                    if (SendInfoAllFileToClient(ref(client[i])) == false)
                        continue;
                    //===========================================Sent file list_file=============================================================

                    //============================================Receive list file need download=====================================================
                    while (true)
                    {
                        ifstream in;
                        if (ReadFileNeedDownFromClient(ref(client[i])) == false)
                            break;
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
                                if (SendInfo1FileToClient(ref(client[i]), temp_2) == false)//đúng rồi khỏi sửa
                                    break;
                                if (Send1FileToClient(ref(client[i]), temp_2) == false)
                                    break;
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
