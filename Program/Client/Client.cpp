// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "Func.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Ham dung de dung ctrl+c
void SignalCallBack(int signum) {
    cout << "Caught signal " << signum << endl;

    exit(signum);
}
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
                cout << "\nClient has connected to the server";
                cout << endl;
                string fileName = "text1.txt";
                ofstream fout;
                fout.open(fileName, ios::out | ios::binary);

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
            }
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
