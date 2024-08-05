// Sever.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Server.h"

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
           
            sever.Listen();
            CSocket client;
            while (true)
            {
                system("cls");
                cout << "\t\t\t\t\t ____  _____ ______     _______ ____\n";
                cout << "\t\t\t\t\t/ ___|| ____|  _ \\ \\   / /____ |  _ \\\n";
                cout << "\t\t\t\t\t\\___ \\|  _| | |_) \\ \\ / /|  _| | |_) |\n";
                cout << "\t\t\t\t\t ___) | |___|  _ < \\ V / | |___|  _ <\n";
                cout << "\t\t\t\t\t|____/|_____|_| \\_\\ \\_/  |_____|_| \\_\\\n";

                cout << "\n================================================================================================================\n";

                cout << "Server is listening!\n";
                if (sever.Accept(client))
                {
                    cout << "Connected is successful!\n";

                    //===========================================Sent file list_file=============================================================
                    if (SendInfoAllFileToClient(ref(client)) == false)
                    {
                        cout << "Disconnected from client" << endl;
                        continue;
                    }
                    //===========================================Sent file list_file=============================================================

                    //============================================Receive list file need download=====================================================
                    while (true)
                    {
                        ifstream in;
                        if (ReadFileNeedDownFromClient(ref(client)) == false)
                        {
                            cout << "Disconnected from client" << endl;
                            break;
                        }
                        cout << "Received list file need to download!\n";
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
                            bool isBreak = false;
                            while (getline(in, temp_2, '\n'))
                            {
                                if (SendInfo1FileToClient(ref(client), temp_2) == false)
                                {
                                    cout << "Disconnected from client" << endl;
                                    isBreak = true;
                                    break;
                                }
                                if (Send1FileToClient(ref(client), temp_2) == false)
                                {
                                    cout << "Disconnected from client" << endl;
                                    isBreak = true;
                                    break;
                                }
                            }
                            in.close();
                            if (isBreak == true)
                                break;
                            //===========Send file ============================================================
                        }
                    }
                    client.Close();
                    //============================================Receive list file need download=====================================================
                }
                else
                {
                    cout << "Connect is falled!\n";
                }
                system("pause");
            }
            /*}*/
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
