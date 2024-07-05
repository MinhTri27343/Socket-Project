// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "afxsock.h"
#include<fstream>
#include<signal.h>
#include<string>
#include<vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;
//Ham dung de dung ctrl+c
void SignalCallBack(int signum) {
    cout << "Caught signal " << signum << endl;
    
    exit(signum);
}
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
                cout << "\nClient has connected to the server\n";

            }
            else
            {
                cout << "\nUnable to connect to the server\n";
            }
            
            //Nhận từng byte rồi đồng thời in ra console và ghi vô file.
            ofstream fout;
            fout.open("test1.txt", ios::out | ios::binary);
            char byte;
            while (client.Receive(&byte, 1, 0))
            {
                fout << byte;
                cout << byte;
            }
            fout.close();
            signal(SIGINT, SignalCallBack);
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
