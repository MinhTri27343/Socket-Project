// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include<conio.h>
#include<functional>
#include"Func.h"
//#pragma pack(push, 1)
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
            AfxSocketInit(NULL);
            CSocket client;
            char ip[100];
            int port;
            client.Create();
            cout << "\t \t \t \t \t  ____ _     ___ _____ _   _ _____ \n";
            cout << "\t \t \t \t \t / ___| |   |_ _| ____| \\ | |_   _|\n";
            cout << "\t \t \t \t \t| |   | |    | ||  _| |  \\| | | |  \n";
            cout << "\t \t \t \t \t| |___| |___ | || |___| |\\  | | |  \n";
            cout << "\t \t \t \t \t \\____|_____|___|_____|_| \\_| |_|  \n";
            // input
            /*cout << "Input IP address of server: "; cin >> ip;
            cout << "Input port of server: "; cin >> port;*/

            if (client.Connect(CA2W("127.0.0.1"), 1234))
            {
                cout << "\nClient has connected to the server";
                cout << endl;
                vector<pair<ofstream, File>> v;
                vector<File> files;
                
                ReceiveInfoAllFileFromServer(ref(client));
                vector<string> list_file = readListFile();
                signal(SIGINT, SignalCallBack);
                //Thread check 2s
                thread check_2s(checkInput, ref(files), list_file);
                check_2s.detach();

                while (true)
                {
                    ReceiveFileDownloadToClient(ref(client), v, files);
                }
                
            }
            else
            {
                cout << "\nUnable to connect to the server";
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
