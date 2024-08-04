// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Server.h"
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
            CSocket sever;
            sever.Create(1234);
            cout << "Server is listening!\n";
            sever.Listen();
            CSocket client;
            if (sever.Accept(client))
            {
                cout << "Connect is successful!\n";
          
                if (SendInfoAllFileToClient(ref(client)) == SOCKET_ERROR)
                {
                    return 0;
                }
                vector<pair<ifstream, File>> v;
                vector<File> tmp;
                isCheckUpdate(ref(client), v);
                while (true)
                {
                    SendFileDownloadToClient(ref(client), v);
                }
            }
            else
            {
                cout << "Connect is falled!\n";
            }
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
